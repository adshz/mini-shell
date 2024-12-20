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

static char *expand_variables(t_shell *shell, const char *arg)
{
	char *result;
	char *var_value;
	size_t i;
	size_t j;
	char var_name[1024];

	if (!arg)
		return (NULL);

	result = malloc(4096);
	if (!result)
		return (NULL);

	i = 0;
	j = 0;

	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1] && (ft_isalnum(arg[i + 1]) || arg[i + 1] == '_'))
		{
			size_t var_len = 0;
			i++;  // Skip the $
			while (arg[i + var_len] && (ft_isalnum(arg[i + var_len]) || arg[i + var_len] == '_'))
				var_len++;
			if (var_len > 0)
			{
				ft_strlcpy(var_name, arg + i, var_len + 1);
				var_value = hashmap_get(shell->env, var_name);
				if (var_value)
				{
					ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
					j += ft_strlen(var_value);
				}
				i += var_len;
				continue;
			}
		}
		result[j++] = arg[i++];
	}
	result[j] = '\0';
	return (result);
}

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

static int	execute_external_command(t_shell *shell, t_ast_node *node)
{
	char    *cmd_path;
	char    **env_array;
	pid_t   pid;
	int     status;
	char    *expanded_cmd;

	// Expand the command if it starts with $
	if (node->args[0][0] == '$')
	{
		expanded_cmd = expand_variables(shell, node->args[0]);
		if (!expanded_cmd)
			return (print_error(node->args[0], MSG_CMD_NOT_FOUND, ERR_CMD_NOT_FOUND));

		// Format error message like bash
		ft_putstr_fd("bash: ", STDERR_FILENO);
		ft_putstr_fd(expanded_cmd, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		free(expanded_cmd);
		return (127);
	}

	cmd_path = get_command_path(node->args[0], shell->env);
	if (!cmd_path)
		return (print_error(node->args[0], MSG_CMD_NOT_FOUND, ERR_CMD_NOT_FOUND));

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}

	shell->signint_child = true;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, node->args, env_array);
		print_error(cmd_path, strerror(errno), ERR_NOT_EXECUTABLE);
		exit(ERR_NOT_EXECUTABLE);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		shell->signint_child = false;
		free(cmd_path);
		ft_free_array(env_array);
		return (WEXITSTATUS(status));
	}

	free(cmd_path);
	ft_free_array(env_array);
	return (print_error(NULL, "fork failed", ERR_GENERAL));
}

int	execute_command(t_ast_node *node, t_hashmap *env)
{
	t_shell	shell;

	if (!node->args || !node->args[0])
		return (1);
	
	shell.env = env;
	shell.signint_child = false;
	
	if (is_builtin(node->args[0]))
	{
		int ret = execute_builtin(&shell, node);
		if (ft_strcmp(node->args[0], "exit") == 0)
			exit(ret);
		return ret;
	}
	return (execute_external_command(&shell, node));
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
	if (!node)
		return (0);

	if (node->type == AST_COMMAND)
		return (execute_command(node, shell->env));
	else if (node->type == AST_PIPE)
		return (execute_pipe(shell, node));
	else if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT ||
			node->type == AST_REDIR_APPEND || node->type == AST_HEREDOC)
		return (execute_redirection(shell, node));

	return (1);
}
