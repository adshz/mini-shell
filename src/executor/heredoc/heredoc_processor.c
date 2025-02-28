/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:52:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:10:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"


static bool	should_leave_leaf(t_shell *shell, int fd[2], int *process_info)
{
	int	status;

	waitpid(*process_info, &status, 0);
	close(fd[WRITE_END]);
	if (exited_normally(status) && get_exit_status(status) == HEREDOC_CTRL_C)
	{
		g_signal_status = SHELL_STATE_HEREDOC_INTERRUPTED;
		close(fd[READ_END]);
		return (true);
	}
	return (false);
}

static bool	handle_leaf(t_shell *shell, t_ast_node *ast_node)
{
	t_io_node	*io;
	int			fd[2];
	int			pid;

	if (ast_node->raw_command)
		ast_node->expanded_argv = expand_raw_command(shell, ast_node->raw_command);
	io = ast_node->io_list;
	while (io)
	{
		if (io->type == IO_HEREDOC)
		{
			pipe(fd);
			g_signal_status = SHELL_STATE_HEREDOC_INPUT;
			pid = (signal(SIGQUIT, SIG_IGN), fork());
			if (pid == 0)
				heredoc_handler(shell, io, fd);
			if (should_leave_leaf(fd, &pid))
				return (true);
			io->here_doc = fd[READ_END];
		}
		else
			io->expanded_value = expand_raw_command(shell, io->value);
		io = io->next;
	}
	return (false);
}

bool	traverse_expand_tree(t_shell *shell, t_ast_node *ast_node)
{
	bool	heredoc_interrupted;

	if (!ast_node)
		return (false);
	if (ast_node->type == NODE_PIPE || ast_node->type == NODE_AND \
		|| ast_node->type == NODE_OR)
	{
		heredoc_interrupted = traverse_expand_tree(shell, ast_node->left);
		if (heredoc_interrupted)
			return (true);
		return (traverse_expand_tree(shell, ast_node->right));
	}
	else
		return (handle_leaf(shell, ast_node));
}

