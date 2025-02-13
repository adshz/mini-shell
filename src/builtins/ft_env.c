/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:48:53 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 22:04:57 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "executor/executor.h"
#include "libft.h"
#include "hashtable/hashtable.h"
#include "utils/utils.h"

static void	print_env_var(const char *key, const char *value)
{
	char	*key_copy;
	char	*value_copy;

	key_copy = ft_strdup(key);
	value_copy = ft_strdup(value);
	ft_putstr_fd(key_copy, STDOUT_FILENO);
	ft_putstr_fd("=", STDOUT_FILENO);
	ft_putendl_fd(value_copy, STDOUT_FILENO);
	free(key_copy);
	free(value_copy);
}

static int	execute_env_command(t_shell *shell,
			char *cmd_path, t_ast_node *node)
{
	char	**env_array;
	pid_t	pid;
	int		status;

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		return (1);
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmd_path, node->args + 1, env_array);
		exit(127);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		free(cmd_path);
		ft_free_array(env_array);
		return ((status >> 8) & 0xff);
	}
	free(cmd_path);
	ft_free_array(env_array);
	return (1);
}

static int	handle_env_command_not_found(const char *cmd)
{
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (127);
}

int	builtin_env(t_shell *shell, t_ast_node *node)
{
	char	*cmd_path;

	if (!node || !node->args)
		return (1);
	if (!node->args[1])
	{
		hashmap_iterate(shell->env, print_env_var);
		return (0);
	}
	cmd_path = get_command_path(shell, node->args[1], shell->env);
	if (!cmd_path)
		return (handle_env_command_not_found(node->args[1]));
	return (execute_env_command(shell, cmd_path, node));
}
