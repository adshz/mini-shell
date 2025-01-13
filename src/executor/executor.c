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

	ft_putstr_fd("[DEBUG] Executing command: ", STDERR_FILENO);
	ft_putendl_fd(node->args[0], STDERR_FILENO);

	// Handle variable expansions in command name and arguments
	char *cmd_name = NULL;
	size_t i = 0;
	while (node->args[i])
	{
		ft_putstr_fd("[DEBUG] Checking arg: ", STDERR_FILENO);
		ft_putendl_fd(node->args[i], STDERR_FILENO);

		char *expanded = NULL;
		if (ft_strchr(node->args[i], '$'))
		{
			expanded = expand_complex_variable(shell, node->args[i]);
			if (expanded)
			{
				ft_putstr_fd("[DEBUG][MEMORY] Got expanded value: ", STDERR_FILENO);
				ft_putendl_fd(expanded, STDERR_FILENO);
				// For command name (i == 0), we need to handle it specially
				if (i == 0)
				{
					// Find the position of $ in the original command
					char *dollar_pos = ft_strchr(node->args[0], '$');
					if (dollar_pos)
					{
						// Get the prefix before $
						size_t prefix_len = dollar_pos - node->args[0];
						ft_putstr_fd("[DEBUG][MEMORY] Allocating prefix of length: ", STDERR_FILENO);
						ft_putnbr_fd(prefix_len, STDERR_FILENO);
						ft_putchar_fd('\n', STDERR_FILENO);
						char *prefix = ft_substr(node->args[0], 0, prefix_len);
						if (!prefix)
						{
							ft_putstr_fd("[DEBUG][MEMORY] Failed to allocate prefix\n", STDERR_FILENO);
							free(expanded);
							return (1);
						}
						ft_putstr_fd("[DEBUG][MEMORY] Allocated prefix: ", STDERR_FILENO);
						ft_putendl_fd(prefix, STDERR_FILENO);

						// Join prefix with first word of expanded value
						char *first_word = expanded;
						char *space_pos = ft_strchr(expanded, ' ');
						if (space_pos)
						{
							ft_putstr_fd("[DEBUG][MEMORY] Found space in expanded value\n", STDERR_FILENO);
							*space_pos = '\0';  // Temporarily terminate at space
						}
						ft_putstr_fd("[DEBUG][MEMORY] Joining prefix with first word\n", STDERR_FILENO);
						cmd_name = ft_strjoin(prefix, first_word);
						if (space_pos)
						{
							*space_pos = ' ';  // Restore the space
							// Create a new argument for the rest
							ft_putstr_fd("[DEBUG][MEMORY] Allocating rest of expanded value\n", STDERR_FILENO);
							char *rest = ft_strdup(space_pos + 1);
							if (rest)
							{
								ft_putstr_fd("[DEBUG][MEMORY] Allocated rest: ", STDERR_FILENO);
								ft_putendl_fd(rest, STDERR_FILENO);
								// Shift all arguments right by one
								int j = 1;
								while (node->args[j])
									j++;
								node->args[j + 1] = NULL;
								while (j > 1)
								{
									node->args[j] = node->args[j - 1];
									j--;
								}
								ft_putstr_fd("[DEBUG][MEMORY] Freeing old args[1]\n", STDERR_FILENO);
								if (node->args[1])
									free(node->args[1]);
								node->args[1] = rest;
							}
						}
						ft_putstr_fd("[DEBUG][MEMORY] Freeing prefix\n", STDERR_FILENO);
						free(prefix);
						ft_putstr_fd("[DEBUG][MEMORY] Freeing expanded\n", STDERR_FILENO);
						free(expanded);
						if (!cmd_name)
						{
							ft_putstr_fd("[DEBUG][MEMORY] Failed to create cmd_name\n", STDERR_FILENO);
							return (1);
						}
						ft_putstr_fd("[DEBUG][MEMORY] Freeing old args[0]\n", STDERR_FILENO);
						free(node->args[0]);
						node->args[0] = cmd_name;
					}
					else
					{
						ft_putstr_fd("[DEBUG][MEMORY] Freeing old args[0]\n", STDERR_FILENO);
						free(node->args[0]);
						node->args[0] = expanded;
					}
				}
				else
				{
					ft_putstr_fd("[DEBUG][MEMORY] Freeing old args[", STDERR_FILENO);
					ft_putnbr_fd(i, STDERR_FILENO);
					ft_putendl_fd("]\n", STDERR_FILENO);
					free(node->args[i]);
					node->args[i] = expanded;
				}
				ft_putstr_fd("[DEBUG] Expanded to: ", STDERR_FILENO);
				ft_putendl_fd(node->args[i], STDERR_FILENO);
			}
		}
		i++;
	}

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
			return (exit_status);
		}
		return (1);
	}
	else
	{
		free(cmd_path);
		ft_free_array(env_array);
		shell->exit_status = ERR_FORK;
		return (print_error(NULL, MSG_FORK, ERR_FORK));
	}
}

int	execute_command(t_shell *shell, t_ast_node *node)
{
	if (!node || !node->args || !node->args[0])
		return 1;

	int ret;

	if (is_builtin(node->args[0]))
	{
		ft_putstr_fd("[DEBUG] Executing builtin: ", STDERR_FILENO);
		ft_putendl_fd(node->args[0], STDERR_FILENO);
		
		ret = execute_builtin(shell, node);
		
		if (ft_strcmp(node->args[0], "exit") == 0)
		{
			ft_putstr_fd("[DEBUG] Exit command, in_heredoc=", STDERR_FILENO);
			ft_putendl_fd(shell->in_heredoc ? "true" : "false", STDERR_FILENO);
			
			if (!shell->in_heredoc)
			{
				ft_putendl_fd("[DEBUG] Executing exit", STDERR_FILENO);
				exit(ret);
			}
			else
			{
				ft_putendl_fd("[DEBUG] Skipping exit due to heredoc", STDERR_FILENO);
			}
		}
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

static int handle_variable_assignment(t_shell *shell, const char *assignment)
{
	char *equals;
	char *key;
	char *value;

	ft_putstr_fd("[DEBUG] Handling variable assignment: ", STDERR_FILENO);
	ft_putendl_fd((char *)assignment, STDERR_FILENO);

	equals = ft_strchr(assignment, '=');
	if (!equals)
		return (1);

	key = ft_substr(assignment, 0, equals - assignment);
	value = ft_strdup(equals + 1);

	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}

	ft_putstr_fd("[DEBUG] Setting variable: ", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("=", STDERR_FILENO);
	ft_putendl_fd(value, STDERR_FILENO);

	hashmap_set(shell->env, key, value);
	free(key);
	free(value);
	return (0);
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	int ret;
	pid_t pid;
	int status;
	int saved_stdout;

	if (!node)
		return (0);

	if (node->type == AST_VAR_EXPANSION)
	{
		ret = handle_variable_assignment(shell, node->value);
		shell->exit_status = ret;
		return ret;
	}

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
			// Special handling for cd: execute in current process
			if (ft_strcmp(node->left->args[0], "cd") == 0)
			{
				// Save current stdout
				saved_stdout = dup(STDOUT_FILENO);
				if (saved_stdout == -1)
					return (print_error(NULL, "dup failed", 1));

				// Set up redirection
				setup_redirections(shell, node);
				
				// Execute cd
				ret = execute_builtin(shell, node->left);
				
				// Restore stdout
				if (dup2(saved_stdout, STDOUT_FILENO) == -1)
					ret = print_error(NULL, "dup2 failed", 1);
				close(saved_stdout);
				
				shell->exit_status = ret;
				return ret;
			}
			// For other builtins, fork and handle redirection
			pid = fork();
			if (pid == -1)
				return (print_error(NULL, "fork failed", 1));
			if (pid == 0)
			{
				setup_redirections(shell, node);
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
