/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_io_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:59:01 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:59:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

static int	cleanup_and_error(int fd, int *pipe_fd, char *content, char *msg)
{
	if (fd != -1)
		close(fd);
	if (pipe_fd)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (content)
	{
		unlink(content);
		free(content);
	}
	return (print_error("heredoc", msg, 1));
}

static int	write_to_pipe(int pipe_fd, int content_fd)
{
	char	buffer[4096];
	ssize_t	bytes_read;

	while ((bytes_read = read(content_fd, buffer, sizeof(buffer))) > 0)
	{
		if (write(pipe_fd, buffer, bytes_read) != bytes_read)
			return (1);
	}
	return (bytes_read < 0);
}

int	setup_pipe_and_write(t_heredoc_data *data)
{
	int	pipe_fds[2];
	int	content_fd;

	if (pipe(pipe_fds) == -1)
		return (cleanup_and_error(-1, NULL, data->content_path, "pipe failed"));

	content_fd = open(data->content_path, O_RDONLY);
	if (content_fd == -1)
		return (cleanup_and_error(-1, pipe_fds, data->content_path, "failed to open content"));

	if (write_to_pipe(pipe_fds[1], content_fd))
		return (cleanup_and_error(content_fd, pipe_fds, data->content_path, "write failed"));

	close(content_fd);
	close(pipe_fds[1]);

	if (dup2(pipe_fds[0], STDIN_FILENO) == -1)
		return (cleanup_and_error(-1, pipe_fds, data->content_path, "dup2 failed"));

	close(pipe_fds[0]);
	return (0);
}

static int	process_line(char *line, t_ast_node *node, t_heredoc_content *hdc)
{
	size_t	len;

	if (!line)
		return (-1);  // Error
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
	{
		line[len - 1] = '\0';
		len--;
	}
	if (ft_strcmp(line, node->right->value) == 0)
	{
		free(line);
		return (1);  // Found delimiter
	}
	if (append_line_to_content(hdc->content, hdc->content_size, \
							line, hdc->content_capacity))
	{
		ft_putstr_fd("DEBUG: Error in append_line_to_content\n", STDERR_FILENO);
		free(line);
		return (-1);  // Error
	}
	free(line);
	return (0);  // Continue reading
}

static void	cleanup_heredoc_resources(t_heredoc_content *hdc)
{
	if (hdc && hdc->content && *hdc->content)
	{
		unlink(*hdc->content);
		free(*hdc->content);
		*hdc->content = NULL;
	}
}

int	read_heredoc_content(t_ast_node *node, t_heredoc_content *hdc)
{
	char	*line;
	int		result;

	while (1)
	{
		if (hdc->shell->heredoc_sigint)
			return (cleanup_and_error(-1, NULL, *hdc->content, "interrupted"));
		write(STDOUT_FILENO, "> ", 2);
		line = get_next_line(STDIN_FILENO);
		
		if (!line || hdc->shell->heredoc_sigint)
		{
			if (line)
				free(line);
			return (cleanup_and_error(-1, NULL, *hdc->content, "no input or interrupted"));
		}

		result = process_line(line, node, hdc);
		if (result < 0)  // Error case
			return (cleanup_and_error(-1, NULL, *hdc->content, "process line failed"));
		if (result > 0)  // Delimiter found
		{
			cleanup_heredoc_resources(hdc);  // Clean up normally on success
			return (0);
		}
	}
	return (0);
}

