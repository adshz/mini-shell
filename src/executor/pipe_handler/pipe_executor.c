/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:18:31 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 15:30:00 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "parser/parser.h"

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipe_fd[2];
	int		statuses[2];
	pid_t	pids[2];

	statuses[0] = 0;
	statuses[1] = 0;
	shell->in_pipe = true;
	if (setup_pipe(pipe_fd) != 0)
		return (1);
	if (create_pipe_children(shell, node, pipe_fd, pids) != 0)
		return (1);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_for_children(pids, statuses);
	shell->in_pipe = false;
	return (handle_child_exit_status(statuses[0], statuses[1]));
}
