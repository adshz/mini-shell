/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_content_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:58:16 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:58:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include <fcntl.h>
#include <math.h>

static void	init_template(char *template, int index)
{
	int		i;
	char	*base;

	base = "/tmp/.heredoc_";
	i = 0;
	while (base[i])
	{
		template[i] = base[i];
		i++;
	}
	// Add index as a 6-digit number
	template[i++] = '0' + ((index / 100000) % 10);
	template[i++] = '0' + ((index / 10000) % 10);
	template[i++] = '0' + ((index / 1000) % 10);
	template[i++] = '0' + ((index / 100) % 10);
	template[i++] = '0' + ((index / 10) % 10);
	template[i++] = '0' + (index % 10);
	template[i] = '\0';
}

static int	create_temp_file(char *template)
{
	int	fd;
	int	i;

	// Check if /tmp is accessible
	if (access("/tmp", W_OK) == -1)
	{
		ft_putstr_fd("DEBUG: /tmp directory is not writable\n", STDERR_FILENO);
		return (-1);
	}
	i = 0;
	while (i < 1000)
	{
		init_template(template, i);
		fd = open(template, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			return (fd);
		if (errno != EEXIST)
			return (-1);
		i++;
	}
	return (-1);
}

int	init_heredoc_content(char **content)
{
	char	template[21];
	int		fd;

	ft_putstr_fd("DEBUG: Initializing heredoc content\n", STDERR_FILENO);
	fd = create_temp_file(template);
	if (fd == -1)
	{
		ft_putstr_fd("DEBUG: Failed to create temp file\n", STDERR_FILENO);
		return (print_error("heredoc", "failed to create temp file", 1));
	}
	ft_putstr_fd("DEBUG: Created temp file: [", STDERR_FILENO);
	ft_putstr_fd(template, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	
	close(fd);
	*content = ft_strdup(template);
	if (!*content)
	{
		ft_putstr_fd("DEBUG: Failed to duplicate temp file path\n", STDERR_FILENO);
		unlink(template);
		return (print_error("heredoc", "malloc failed", 1));
	}
	ft_putstr_fd("DEBUG: Successfully initialized heredoc content\n", STDERR_FILENO);
	return (0);
}

int	resize_content_buffer(char **content, size_t *content_capacity, \
	size_t needed_size, char *line)
{
	char	*new_content;

	if (needed_size <= *content_capacity)
		return (0);
	new_content = malloc(*content_capacity * 2);
	if (!new_content)
	{
		free(*content);
		free(line);
		return (print_error("heredoc", "malloc failed", 1));
	}
	ft_strlcpy(new_content, *content, *content_capacity);
	free(*content);
	*content = new_content;
	*content_capacity *= 2;
	return (0);
}

static int	write_line_to_fd(int fd, char *line)
{
	ssize_t	write_result;

	ft_putstr_fd("DEBUG: Writing line to fd: [", STDERR_FILENO);
	ft_putstr_fd(line, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	
	write_result = write(fd, line, ft_strlen(line));
	if (write_result == -1)
	{
		ft_putstr_fd("DEBUG: Failed to write line content\n", STDERR_FILENO);
		return (-1);
	}
	write_result = write(fd, "\n", 1);
	if (write_result == -1)
	{
		ft_putstr_fd("DEBUG: Failed to write newline\n", STDERR_FILENO);
		return (-1);
	}
	ft_putstr_fd("DEBUG: Successfully wrote line to fd\n", STDERR_FILENO);
	return (0);
}

int	append_line_to_content(char **content, size_t *content_size, \
	char *line, size_t *content_capacity)
{
	int		fd;
	size_t	len;

	ft_putstr_fd("DEBUG: Appending line to content file: [", STDERR_FILENO);
	ft_putstr_fd(*content, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	
	(void)content_capacity;
	(void)content_size;
	fd = open(*content, O_WRONLY | O_APPEND);
	if (fd == -1)
	{
		ft_putstr_fd("DEBUG: Failed to open temp file for appending\n", STDERR_FILENO);
		return (print_error("heredoc", "failed to open temp file", 1));
	}
	ft_putstr_fd("DEBUG: Opened temp file for appending\n", STDERR_FILENO);
	
	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		line[len - 1] = '\0';
	
	if (write_line_to_fd(fd, line) == -1)
	{
		close(fd);
		ft_putstr_fd("DEBUG: Failed to write line to temp file\n", STDERR_FILENO);
		return (print_error("heredoc", "write failed", 1));
	}
	ft_putstr_fd("DEBUG: Successfully wrote line to temp file\n", STDERR_FILENO);
	
	close(fd);
	return (0);
}

int init_heredoc_data(t_heredoc_data *data, const char *delimiter)
{
	char template[21];
	int fd;

	if (!data || !delimiter)
		return (1);
	
	fd = create_temp_file(template);
	if (fd == -1)
		return (1);
	close(fd);

	data->content_path = ft_strdup(template);
	if (!data->content_path)
	{
		unlink(template);
		return (1);
	}
	data->delimiter = delimiter;
	data->content_fd = -1;
	return (0);
}

void cleanup_heredoc_data(t_heredoc_data *data)
{
	if (!data)
		return;
	if (data->content_fd != -1)
		close(data->content_fd);
	if (data->content_path)
	{
		unlink(data->content_path);
		free(data->content_path);
	}
}
