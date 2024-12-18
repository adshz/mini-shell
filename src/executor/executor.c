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
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
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

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		return (1);
	}
	execve(cmd_path, node->args, env_array);
	perror("execve");
	free(cmd_path);
	ft_free_array(env_array);
	return (1);
}

int	execute_command(char **args, t_env *env)
{
	char	*cmd_path;
	int		status;
	pid_t	pid;

	if (!args || !args[0])
		return (1);
	
	// First check if it's a builtin
	if (is_builtin(args[0]))
		return (execute_builtin(args, env));

	// Get the full path of the command
	cmd_path = get_command_path(args[0], env);
	if (!cmd_path)
	{
		ft_error("command not found", args[0]);
		return (127);  // Command not found error code
	}

	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (ft_error("fork failed", NULL));
	}
	if (pid == 0)
	{
		execve(cmd_path, args, env_to_array(env));
		// If execve returns, there was an error
		ft_error("execve failed", args[0]);
		exit(126);  // Execution permission denied or similar
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	return (WEXITSTATUS(status));
}

// Helper function to find the full path of a command
static char	*get_command_path(char *cmd, t_env *env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	path_var = get_env_value(env, "PATH");
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
		exit(execute_command(node->args, shell->env));
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
		return (handle_pipe(shell, node));
	return (0);
}

