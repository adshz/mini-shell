/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:52:18 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:52:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor.h"
#include "builtins.h"
#include "libft.h"
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>
#include "errors.h"
#include "expander.h"

static char	*create_env_string(const char *key, const char *value)
{
	return (ft_strjoin3(key, "=", value));
}

static int	fill_env_array(t_hashmap *env, char **env_array)
{
	t_hash_item	*item;
	size_t		i;
	size_t		j;

	i = 0;
	j = 0;
	while (j < env->size)
	{
		item = env->items[j];
		while (item)
		{
			if (item->key && item->value)
			{
				env_array[i] = create_env_string(item->key, item->value);
				if (!env_array[i])
					return (0);
				i++;
			}
			item = item->next;
		}
		j++;
	}
	env_array[i] = NULL;
	return (1);
}

char	**create_env_array(t_hashmap *env)
{
	char	**env_array;
	size_t	count;

	count = hashmap_size(env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env, env_array))
	{
		ft_free_array(env_array);
		return (NULL);
	}
	return (env_array);
}

static int execute_external_command(t_shell *shell, t_ast_node *node)
{
	char **cmd_args;
	char *cmd_path;
	int status;

	if (node->args[0][0] == '$')
	{
		cmd_args = expand_command(shell, node->args[0]);
		if (!cmd_args)
		{
			shell->exit_status = ERR_CMD_NOT_FOUND;
			return print_error(node->args[0], MSG_CMD_NOT_FOUND, ERR_CMD_NOT_FOUND);
		}
	}
	else
		cmd_args = node->args;

	cmd_path = get_command_path(cmd_args[0], shell->env);
	if (!cmd_path)
	{
		if (node->args[0][0] == '$')
			free_expanded_args(cmd_args);
		return print_error(cmd_args[0], MSG_CMD_NOT_FOUND, ERR_CMD_NOT_FOUND);
	}

	char    **env_array;
	pid_t   pid;
	int     exit_status;

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		if (node->args[0][0] == '$')
			ft_free_array(cmd_args);
		shell->exit_status = ERR_MALLOC;
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}

	shell->signint_child = true;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, cmd_args, env_array);
		print_error(cmd_path, strerror(errno), ERR_NOT_EXECUTABLE);
		exit(ERR_NOT_EXECUTABLE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		shell->signint_child = false;
		free(cmd_path);
		ft_free_array(env_array);
		if (node->args[0][0] == '$')
			ft_free_array(cmd_args);
		if (WIFEXITED(status))
		{
			exit_status = WEXITSTATUS(status);
			shell->exit_status = exit_status;
			return exit_status;
		}
		shell->exit_status = 1;
		return 1;
	}

	free(cmd_path);
	ft_free_array(env_array);
	if (node->args[0][0] == '$')
		ft_free_array(cmd_args);
	shell->exit_status = ERR_GENERAL;
	return (print_error(NULL, "fork failed", ERR_GENERAL));
}

int	execute_command(t_shell *shell, t_ast_node *node)
{
	int ret;

	if (!node->args || !node->args[0])
		return (1);
	
	if (is_builtin(node->args[0]))
	{
		ret = execute_builtin(shell, node);
		if (ft_strcmp(node->args[0], "exit") == 0)
			exit(ret);
		return ret;
	}
	ret = execute_external_command(shell, node);
	return ret;
}

char	*get_command_path(const char *cmd, t_hashmap *env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	if (!cmd || !*cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	
	path_var = hashmap_get(env, "PATH");
	if (!path_var)
		return (NULL);
	
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (access(full_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	return (NULL);
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	int ret;

	if (!node)
		return (0);

	if (node->type == AST_COMMAND)
	{
		ret = execute_command(shell, node);
		shell->exit_status = ret;  // Update shell's exit status
		return ret;
	}
	else if (node->type == AST_PIPE)
	{
		ret = execute_pipe(shell, node);
		shell->exit_status = ret;  // Update shell's exit status
		return ret;
	}
	else if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT ||
			node->type == AST_REDIR_APPEND || node->type == AST_HEREDOC)
	{
		ret = execute_redirection(shell, node);
		shell->exit_status = ret;  // Update shell's exit status
		return ret;
	}

	shell->exit_status = 1;  // Update shell's exit status for error case
	return (1);
}
