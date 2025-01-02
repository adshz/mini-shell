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
	if (!node->args[0])
		return (1);

	if (node->args[0][0] == '~')
	{
		char *expanded = expand_tilde(shell, node->args[0]);
		if (!expanded)
		{
			shell->exit_status = 127;
			return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
		}

		if (access(expanded, F_OK) == 0)
		{
			ft_putstr_fd(expanded, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			free(expanded);
			return 1;
		}
		free(expanded);
	}

	if (node->args[0][0] == '$')
	{
		char *expanded_value = expand_simple_variable(shell, node->args[0] + 1);
		if (!expanded_value || !*expanded_value)
		{
			if (expanded_value)
				free(expanded_value);
			return (0);  // Just return success without executing anything
		}

		t_token *expanded_tokens = tokenise(expanded_value);
		free(expanded_value);
		
		if (!expanded_tokens)
		{
			shell->exit_status = 127;
			return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
		}

		t_ast_node *expanded_ast = parse(expanded_tokens);
		free_tokens(expanded_tokens);

		if (!expanded_ast)
		{
			shell->exit_status = 127;
			return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
		}

		int status = execute_ast(shell, expanded_ast);
		free_ast(expanded_ast);
		return status;
	}

	// Special handling for . and ..
	if (ft_strcmp(node->args[0], "..") == 0 || ft_strcmp(node->args[0], ".") == 0)
	{
		shell->exit_status = 127;
		return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
	}

	char *cmd_path = get_command_path(node->args[0], shell->env);
	if (!cmd_path)
	{
		// If the command contains a slash, try to execute it directly
		if (ft_strchr(node->args[0], '/'))
		{
			if (access(node->args[0], F_OK) == 0)
			{
				if (access(node->args[0], X_OK) != 0)
					return print_error(node->args[0], strerror(errno), ERR_NOT_EXECUTABLE);
				cmd_path = ft_strdup(node->args[0]);
			}
			else
			{
				shell->exit_status = 127;
				return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
			}
		}
		else
		{
			shell->exit_status = 127;
			return print_error(node->args[0], MSG_CMD_NOT_FOUND, 127);
		}
	}

	char **env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		shell->exit_status = ERR_MALLOC;
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}

	shell->signint_child = true;
	pid_t pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, node->args, env_array);
		print_error(cmd_path, strerror(errno), ERR_NOT_EXECUTABLE);
		
		exit(ERR_NOT_EXECUTABLE);
	}
	else if (pid > 0)
	{
		int status;
		waitpid(pid, &status, 0);
		shell->signint_child = false;
		free(cmd_path);
		ft_free_array(env_array);
		if (WIFEXITED(status))
		{
			int exit_status = WEXITSTATUS(status);
			shell->exit_status = exit_status;
			return exit_status;
		}
		shell->exit_status = 1;
		return 1;
	}

	free(cmd_path);
	ft_free_array(env_array);
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
	if (ft_strchr(cmd, '/') || ft_strcmp(cmd, "..") == 0 || ft_strcmp(cmd, ".") == 0)
		return (NULL);  // Return NULL for direct paths and . or ..
	
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
	pid_t pid;
	int status;

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
		// Special handling for builtins with redirections
		if (node->left && node->left->args && is_builtin(node->left->args[0]))
		{
			// Fork only for redirections
			pid = fork();
			if (pid == -1)
				return (print_error(NULL, "fork failed", 1));
			if (pid == 0)
			{
				handle_redirections(shell, node);
				ret = execute_builtin(shell, node->left);
				exit(ret);
			}
			waitpid(pid, &status, 0);
			ret = WEXITSTATUS(status);
			shell->exit_status = ret;
			
			// If it's the exit builtin, we need to exit after handling redirection
			if (node->left->args && ft_strcmp(node->left->args[0], "exit") == 0)
				exit(ret);
				
			return ret;
		}
		
		// Normal redirection handling for non-builtins
		ret = execute_redirection(shell, node);
		shell->exit_status = ret;  // Update shell's exit status
		return ret;
	}

	shell->exit_status = 1;  // Update shell's exit status for error case
	return (1);
}
