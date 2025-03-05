/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:51:26 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:07:58 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

void	execute_pipe_child(t_shell *shell, t_ast_node *node, \
							int pipe_ends[2], t_pipe_role role)
{
	int	child_status;

	if (role == PIPE_WRITER)
	{
		close(pipe_ends[READ_END]);
		if (dup2(pipe_ends[WRITE_END], STDOUT_FILENO) == -1)
			exit(1);
		close(pipe_ends[WRITE_END]);
	}
	else if (role == PIPE_READER)
	{
		close(pipe_ends[WRITE_END]);
		if (dup2(pipe_ends[READ_END], STDIN_FILENO) == -1)
			exit(1);
		close(pipe_ends[READ_END]);
	}
	child_status = execute_ast_node(shell, node, true);
	cleanup_minishell(shell);
	exit(child_status);
}
