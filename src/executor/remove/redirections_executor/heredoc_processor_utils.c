/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:35:40 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:35:52 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "utils/utils.h"

int	setup_heredoc(t_ast_node *node, int pipe_fds[2], t_shell *shell)
{
	if (!node || !node->right || !node->right->value)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (pipe(pipe_fds) == -1)
		return (print_error("heredoc", "pipe creation failed", 1));
	g_signal_status = SIG_HEREDOC_MODE;
	shell->in_heredoc = 1;
	return (0);
}

static int	handle_heredoc_interrupt(char *line, int pipe_fds[2],
		t_ast_node *node, t_shell *shell)
{
	close(pipe_fds[0]);
	close(pipe_fds[1]);
	shell->in_heredoc = 0;
	if (g_signal_status == SIG_HEREDOC_INT)
	{
		if (shell->ast && (g_signal_status == SIG_HEREDOC_INT || \
			shell->heredoc_sigint))
			cleanup_heredoc_signal_interrupt(shell->ast);
		shell->heredoc_sigint = true;
		g_signal_status = SIG_NONE;
		shell->exit_status = 130;
		node->data.content_fd = -1;
		if (line)
			free(line);
		return (130);
	}
	if (line)
		free(line);
	node->data.content_fd = -1;
	write(STDERR_FILENO, "\n", 1);
	return (1);
}

int	handle_heredoc_line(char *line, int pipe_fds[2],
		t_ast_node *node, t_shell *shell)
{
	size_t	len;

	len = 0;
	if (!line || g_signal_status == SIG_HEREDOC_INT)
		return (handle_heredoc_interrupt(line, pipe_fds, node, shell));
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	return (0);
}

int	write_heredoc_line(char *line, int pipe_fds[2],
		t_ast_node *node)
{
	if (!line || !node || !node->right || !node->right->value)
	{
		if (line)
			free(line);
		return (1);
	}
	if (ft_strcmp(line, node->right->value) == 0)
	{
		free(line);
		close(pipe_fds[1]);
		write(STDERR_FILENO, "\n", 1);
		return (1);
	}
	ft_putendl_fd(line, pipe_fds[1]);
	free(line);
	return (0);
}
