/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:50:18 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:50:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_pipeline(t_shell *shell, t_ast_node *ast_tree)
{
	int		pipe_ends[2];
	pid_t	pipe_left;
	pid_t	pipe_right;
	int		left_status;
	int		right_status;

	if (pipe(pipe_ends) == -1)
		return (ERRNO_PIPE_FAILURE);
	pipe_left = fork();
	if (pipe_left == -1)
	{
		close(pipe_ends[WRITE_END]);
		close(pipe_ends[READ_END]);
		return (ERRNO_FORK_FAILURE);
	}
	if (pipe_left == 0)
	{
		execute_pipe_child(shell, ast_tree->left, pipe_ends, PIPE_WRITER);
		exit(1);
	}
	pipe_right = fork();
	if (pipe_right == -1)
	{
		close(pipe_ends[WRITE_END]);
		close(pipe_ends[READ_END]);
		kill(pipe_left, SIGTERM);
		waitpid(pipe_left, NULL, 0);
		return (ERRNO_FORK_FAILURE);
	}
	if (pipe_right == 0)
	{
		execute_pipe_child(shell, ast_tree->right, pipe_ends, PIPE_READER);
		exit(1);
	}
	close(pipe_ends[WRITE_END]);
	close(pipe_ends[READ_END]);
	waitpid(pipe_left, &left_status, 0);
	waitpid(pipe_right, &right_status, 0);
	g_signal_status = SHELL_STATE_READLINE;
	return (WEXITSTATUS(right_status));
}


// int	execute_pipeline(t_shell *shell, t_ast_node *ast_tree)
// {
// 	int		pipe_exit_status;
// 	int		pipe_ends[2];
// 	pid_t	pipe_left;
// 	pid_t	pipe_right;

// 	if (pipe(pipe_ends) == -1)
// 		return (ERRNO_PIPE_FAILURE);
// 	pipe_left = fork();
// 	if (pipe_left == -1)
// 		return (close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), ERRNO_FORK_FAILURE);	
// 	if (pipe_left == 0)
// 		execute_pipe_child(shell, ast_tree->left, pipe_ends, PIPE_WRITER);
// 	else
// 	{
// 		pipe_right = fork();
// 		if (pipe_right == -1)
// 			return (close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), ERRNO_FORK_FAILURE);
// 		if (pipe_right == 0)
// 			execute_pipe_child(shell, ast_tree->right, pipe_ends, PIPE_READER);
// 		else
// 		{
// 			(close(pipe_ends[WRITE_END]), close(pipe_ends[READ_END]), \
// 			waitpid(pipe_left, &pipe_exit_status, 0), waitpid(pipe_right, &pipe_exit_status, 0));
// 			g_signal_status = SHELL_STATE_READLINE;
// 			return (exec_get_exit_status(pipe_exit_status));
// 		}
// 	}
// 	return (ERRNO_GENERAL);
// }
