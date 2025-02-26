/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:08:41 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:24:19 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static void	process_heredoc_line(t_shell *shell, t_io_node *io, char *line, \
								int fd)
{
	char	*quotes;

	quotes = io->value;
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	if (!*quotes)
		heredoc_expander(shell, line, fd[WRITE_END]);
	else
	{
		ft_putstr_fd(line, fd[WRITE_END]);
		ft_putstr_fd("\n", fd[WRITE_END]);
	}
}

void	heredoc_handler(t_shell *shell, t_io_node *io, int fd[2])
{
	char	*line;
	char	*quotes;

	quotes = io->value;
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			close(fd[WRITE_END]);
			cleanup_minishell(shell);
			if (errno == HEREDOC_CTRL_D)
				break ;
			else if (g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED)
				exit(HEREDOC_CTRL_C);
		}
		if (is_delimiter(io->value, line))
			break ;
		process_heredoc_line(shell, line, io, fd[WRITE_END]);
		//free(line) may need it but let's experiment it
	}
	cleanup_minishell(shell);
	exit(EXIT_NORMAL);
}
