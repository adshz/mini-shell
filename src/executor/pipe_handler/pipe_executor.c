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
	int		status;
	pid_t	pid1, pid2;

	shell->in_pipe = true;
	if (setup_pipe(pipe_fd) != 0)
		return (1);

	// Execute left side (might be another pipe)
	pid1 = fork();
	if (pid1 == -1)
		return (handle_fork_error(pipe_fd));
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fd[1]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		if (node->left->type == AST_PIPE)
			exit(execute_pipe(shell, node->left));
		else
			exit(execute_ast(shell, node->left));
	}

	// Execute right side
	pid2 = fork();
	if (pid2 == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (1);
	}
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			exit(1);
		close(pipe_fd[0]);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		exit(execute_ast(shell, node->right));
	}

	// Parent process
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	
	// Wait for both children
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	shell->in_pipe = false;
	restore_signal_handlers();
	
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}
