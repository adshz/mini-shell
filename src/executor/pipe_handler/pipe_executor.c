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

int	execute_left_process(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	pid_t	left_pid;
	int		status;

	left_pid = fork();
	if (left_pid == -1)
		return (handle_fork_error(pipe_fd));
	if (left_pid == 0)
		execute_left_child(shell, node, pipe_fd);
	waitpid(left_pid, &status, 0);
	return ((status >> 8) & 0xff);
}

int	execute_right_process(t_shell *shell, t_ast_node *node, \
	int pipe_fd[2], int *right_status)
{
	pid_t	right_pid;
	int		status;

	right_pid = fork();
	if (right_pid == -1)
		return (handle_fork_error(pipe_fd));
	if (right_pid == 0)
		execute_right_child(shell, node, pipe_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(right_pid, &status, 0);
	*right_status = (status >> 8) & 0xff;
	return (0);
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pipe_fd[2];
	int		right_status;
	int		left_status;
	pid_t	left_pid;
	pid_t	right_pid;

	right_status = 0;
	shell->in_pipe = true;
	if (setup_pipe(pipe_fd) != 0)
		return (1);
	left_pid = fork();
	if (left_pid == -1)
	{
		shell->in_pipe = false;
		return (handle_fork_error(pipe_fd));
	}
	if (left_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_left_child(shell, node, pipe_fd);
	}
	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		shell->in_pipe = false;
		return (1);
	}
	if (right_pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_right_child(shell, node, pipe_fd);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(left_pid, &left_status, 0);
	waitpid(right_pid, &right_status, 0);
	restore_signal_handlers();
	shell->in_pipe = false;
	if ((left_status & 0x7f) || (right_status & 0x7f))
	{
		if ((left_status & 0x7f) == SIGQUIT || (right_status & 0x7f) == SIGQUIT)
		{
			if ((right_status & 0x7f) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			return (131);
		}
		if ((left_status & 0x7f) == SIGINT || (right_status & 0x7f) == SIGINT)
			return (130);
	}
	return ((right_status >> 8) & 0xff);
}

void	execute_pipeline(t_shell *shell, t_ast_node *node)
{
	int	pipe_fd[2];
	int	pid1;
	int	pid2;
	int	status;
	int	saved_status;

	if (pipe(pipe_fd) == -1)
		return ;
	pid1 = fork();
	if (pid1 == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return ;
	}
	if (pid1 == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_fd[1]);
		execute_ast_node(shell, node->left);
		exit(shell->exit_status);
	}
	pid2 = fork();
	if (pid2 == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		kill(pid1, SIGTERM);
		waitpid(pid1, NULL, 0);
		return ;
	}
	if (pid2 == 0)
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			exit(1);
		close(pipe_fd[0]);
		execute_ast_node(shell, node->right);
		exit(shell->exit_status);
	}
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	waitpid(pid1, &saved_status, 0);
	waitpid(pid2, &status, 0);
	if (status & 0x7f)
		shell->exit_status = 128 + (status & 0x7f);
	else
		shell->exit_status = (status >> 8) & 0xff;
	return ;
}
