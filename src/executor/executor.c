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

static char	*create_env_string(const char *key, const char *value)
{
	char	*env_str;

	env_str = ft_strjoin3(key, "=", value);
	return (env_str);
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

static int	execute_external_command(t_shell *shell, t_ast_node *node,
		char *cmd_path)
{
	char	**env_array;
	char	*current_dir;

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}

	// Get current working directory from shell's env
	current_dir = hashmap_get(shell->env, "PWD");
	if (current_dir)
	{
		// Change to the current working directory before executing command
		if (chdir(current_dir) != 0)
		{
			ft_putstr_fd("cd: Failed to change directory\n", STDERR_FILENO);
			free(cmd_path);
			ft_free_array(env_array);
			return (1);
		}
	}

	execve(cmd_path, node->args, env_array);
	
	// If execve returns, there was an error
	print_error(cmd_path, strerror(errno), ERR_NOT_EXECUTABLE);
	
	free(cmd_path);
	ft_free_array(env_array);
	return (ERR_NOT_EXECUTABLE);
}

int	execute_command(t_ast_node *node, t_hashmap *env)
{
	char	*cmd_path;
	int		status;
	pid_t	pid;
	t_shell	shell;

	if (!node->args || !node->args[0])
		return (1);
	
	// Initialize shell structure
	shell.env = env;
	shell.signint_child = false;
	
	// First check if it's a builtin
	if (is_builtin(node->args[0]))
		return (execute_builtin(&shell, node));

	// Get the full path of the command
	cmd_path = get_command_path(node->args[0], env);
	if (!cmd_path)
		return (print_error(node->args[0], MSG_CMD_NOT_FOUND, ERR_CMD_NOT_FOUND));

	shell.signint_child = true;  // Mark that we're running a child process
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (print_error(NULL, "fork failed", ERR_GENERAL));
	}
	if (pid == 0)
	{
		status = execute_external_command(&shell, node, cmd_path);
		exit(status);
	}
	
	free(cmd_path);
	waitpid(pid, &status, 0);
	shell.signint_child = false;  // Reset the child process flag
	
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

// Update get_command_path to match the header declaration
char	*get_command_path(const char *cmd, t_hashmap *env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	
	// Use hashmap_get directly instead of converting to env list
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

pid_t	create_process(t_shell *shell)
{
	pid_t	pid;

	(void)shell;
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return (-1);
	}
	return (pid);
}

static int	execute_command_node(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	pid = create_process(shell);
	if (pid == 0)
	{
		handle_redirections(shell, node);
		exit(execute_command(node, shell->env));
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		return (WEXITSTATUS(status));
	}
	return (1);
}

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return (0);
	if (node->type == AST_COMMAND)
		return (execute_command_node(shell, node));
	else if (node->type == AST_PIPE)
		return (execute_pipe(shell, node));
	else if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT ||
			node->type == AST_REDIR_APPEND || node->type == AST_HEREDOC)
		return (execute_redirection(shell, node));
	return (1);
}

