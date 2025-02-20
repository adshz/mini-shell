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
	int		ret;

	shell->in_pipe = true;
	if (setup_pipe(pipe_fd) != 0)
		return (1);
	pids[0] = fork();
	if (pids[0] == -1)
		return (handle_fork_error(pipe_fd));
	if (pids[0] == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			cleanup_current_command(shell);
			exit(1);
		}
		close(pipe_fd[1]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (node->left->type == AST_PIPE)
			ret = execute_pipe(shell, node->left);
		else
			ret = execute_ast(shell, node->left);
		cleanup_current_command(shell);
		exit(ret);
	}
	pids[1] = fork();
	if (pids[1] == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (pids[1] == 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		{
			cleanup_current_command(shell);
			exit(1);
		}
		close(pipe_fd[0]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ret = execute_ast(shell, node->right);
		cleanup_current_command(shell);
		exit(ret);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	wait_for_children(pids, statuses);
	shell->in_pipe = false;
	return (handle_child_exit_status(statuses[0], statuses[1]));
}
