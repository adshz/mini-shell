/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:25:06 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:25:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "utils/utils.h"

int	collect_heredoc_content(t_ast_node *node, t_shell *shell)
{
	int		pipe_fds[2];
	char	*line;
	int		ret;

	ret = setup_heredoc(node, pipe_fds, shell);
	if (ret != 0)
		return (ret);
	while (1)
	{
		write(STDERR_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		ret = handle_heredoc_line(line, pipe_fds, node, shell);
		if (ret != 0)
		{
			shell->in_heredoc = 0;
			g_signal_status = SIG_NONE;
			return (ret);
		}
		if (write_heredoc_line(line, pipe_fds, node))
			break ;
	}
	node->data.content_fd = pipe_fds[0];
	shell->in_heredoc = 0;
	g_signal_status = SIG_NONE;
	return (0);
}

int	setup_heredoc_pipe(t_ast_node *node)
{
	if (!node || node->data.content_fd < 0)
		return (print_error("heredoc", "invalid file descriptor", 1));
	if (dup2(node->data.content_fd, STDIN_FILENO) == -1)
	{
		close(node->data.content_fd);
		return (print_error("heredoc", "dup2 failed", 1));
	}
	close(node->data.content_fd);
	return (0);
}

int	handle_heredoc(t_ast_node *node, t_shell *shell)
{
	int	ret;

	ret = collect_heredoc_content(node, shell);
	return (ret);
}
