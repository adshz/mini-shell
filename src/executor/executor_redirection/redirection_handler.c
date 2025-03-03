/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:52:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:53:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

void	reset_stds(t_shell *shell, bool is_pipe)
{
	if (is_pipe)
		return ;
	dup2(shell->stdin, STDIN_FILENO);
	dup2(shell->stdout, STDOUT_FILENO);
}

int	check_redirection(t_ast_node *node)
{
	t_io_node	*current_io;
	int			status;

	current_io = node->io_list;
	while (current_io)
	{
		if (current_io->type == IO_OUT && \
			ft_handle_output_redirection(current_io, &status) != ERRNO_NONE)
			return (status);
		else if (current_io->type == IO_IN && \
				ft_handle_input_redirection(current_io, &status) != ERRNO_NONE)
			return (status);
		else if (current_io->type == IO_APPEND && \
				ft_handle_append_redirection(current_io, &status) != ERRNO_NONE)
			return (status);
		else if (current_io->type == IO_HEREDOC)
			(dup2(current_io->here_doc, STDOUT_FILENO), \
				close(current_io->here_doc));
		current_io = current_io->next;
	}
	return (ERRNO_NONE);
}
