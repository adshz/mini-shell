/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:57:28 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 16:57:40 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	collect_heredoc_content(t_ast_node *node, t_shell *shell)
{
	int		pipe_fds[2];
	char	*line;
	size_t	len;

	if (!node || !node->right || !node->right->value)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (pipe(pipe_fds) == -1)
		return (print_error("heredoc", "pipe creation failed", 1));
	g_signal_status = SIG_HEREDOC_MODE;
	shell->in_heredoc = 1;
	while (1)
	{
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		if (!line || g_signal_status == SIG_HEREDOC_INT)
		{
			close(pipe_fds[0]);
			close(pipe_fds[1]);
			shell->in_heredoc = 0;
			if (g_signal_status == SIG_HEREDOC_INT)
			{
				shell->heredoc_sigint = true;
				g_signal_status = SIG_NONE;
				shell->exit_status = 130;
			}
			if (line)
				free(line);
			return (130);
		}
		len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';
		if (ft_strcmp(line, node->right->value) == 0)
		{
			free(line);
			close(pipe_fds[1]);
			break;
		}
		write(pipe_fds[1], line, ft_strlen(line));
		write(pipe_fds[1], "\n", 1);
		free(line);
	}
	node->data.content_fd = pipe_fds[0];
	shell->in_heredoc = 0;
	g_signal_status = SIG_NONE;
	return (0);
}

int	setup_heredoc_pipe(t_ast_node *node)
{
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
	return (collect_heredoc_content(node, shell));
}
