/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_process_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:14:58 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:15:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "core/core.h"

int	create_left_child(t_shell *shell, t_ast_node *node,
		int pipe_fd[2], pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		shell->in_pipe = false;
		return (handle_fork_error(pipe_fd));
	}
	if (*pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_left_child(shell, node, pipe_fd);
	}
	return (0);
}

int	create_right_child(t_shell *shell, t_ast_node *node,
		int pipe_fd[2], pid_t *pid)
{
	*pid = fork();
	if (*pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->in_pipe = false;
		return (1);
	}
	if (*pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_right_child(shell, node, pipe_fd);
	}
	return (0);
}

int	create_pipe_children(t_shell *shell, t_ast_node *node,
		int pipe_fd[2], pid_t *pids)
{
	if (create_left_child(shell, node, pipe_fd, &pids[0]) != 0)
		return (1);
	if (create_right_child(shell, node, pipe_fd, &pids[1]) != 0)
		return (1);
	return (0);
}

void	wait_for_children(pid_t *pids, int *statuses)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pids[0], &statuses[0], 0);
	waitpid(pids[1], &statuses[1], 0);
	restore_signal_handlers();
}
