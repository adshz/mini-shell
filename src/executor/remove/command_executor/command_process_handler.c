/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_process_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:04:41 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 09:14:14 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_command_with_path(t_shell *shell, t_ast_node *node, \
	char *cmd_path)
{
	char	**env_array;
	pid_t	pid;
	int		ret;

	env_array = get_env_array(shell);
	if (!env_array)
	{
		free(cmd_path);
		shell->exit_status = ERR_MALLOC;
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}
	shell->signint_child = true;
	pid = fork();
	if (pid == 0)
		return (execute_command_child(node, cmd_path, env_array));
	ft_free_array(env_array);
	ret = handle_parent_process(shell, pid, cmd_path);
	return (ret);
}

int	handle_parent_process(t_shell *shell, pid_t pid, \
	char *cmd_path)
{
	int	status;
	int	exit_status;

	if (pid < 0)
	{
		free(cmd_path);
		shell->exit_status = ERROR_FORK;
		return (print_error(NULL, MSG_FORK, ERROR_FORK));
	}
	waitpid(pid, &status, 0);
	shell->signint_child = false;
	free(cmd_path);
	if (was_signaled(status))
	{
		exit_status = get_signal_from_status(status);
		shell->exit_status = exit_status;
		return (exit_status);
	}
	return (1);
}

int	execute_command_child(t_ast_node *node, char *cmd_path, char **env_array)
{
	execve(cmd_path, node->args, env_array);
	perror("minishell: execve");
	free(cmd_path);
	exit(127);
}
