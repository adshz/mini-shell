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
#include "env.h"

static int	execute_env_command(t_shell *shell,
			char *cmd_path, t_ast_node *node)
{
	char	**env_array;
	pid_t	pid;

	env_array = NULL;
	pid = 0;
	env_array = get_env_array(shell);
	if (!env_array)
	{
		free(cmd_path);
		return (1);
	}
	pid = fork();
	if (pid == 0)
		handle_env_child_process(cmd_path, node->args + 1, env_array);
	else if (pid > 0)
	{
		ft_free_array(env_array);
		return (handle_env_parent_process(pid, cmd_path));
	}
	ft_free_array(env_array);
	free(cmd_path);
	return (1);
}

int	builtin_env(t_shell *shell, t_ast_node *node)
{
	char	*cmd_path;

	if (!node || !node->args)
		return (1);
	if (!node->args[1])
	{
		hashmap_iterate(shell->env, env_print_env_var);
		return (0);
	}
	cmd_path = get_command_path(shell, node->args[1], shell->env);
	if (!cmd_path)
		return (handle_env_command_not_found(node->args[1]));
	return (execute_env_command(shell, cmd_path, node));
}
