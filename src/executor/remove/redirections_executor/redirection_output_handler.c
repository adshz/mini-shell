/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_output_handler.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:29:40 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:30:41 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	open_output_file(const char *expanded_filename, int *fd)
{
	*fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
	{
		print_error((char *)expanded_filename, "Failed to open file", 1);
		return (1);
	}
	return (0);
}

int	setup_file_descriptor(int fd, char *expanded_filename)
{
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_error("dup2", "Failed to duplicate file descriptor", 1);
		close(fd);
		free(expanded_filename);
		return (1);
	}
	close(fd);
	free(expanded_filename);
	return (0);
}

int	handle_output_redirection(t_ast_node *node, t_shell *shell)
{
	char	*filename;
	char	*expanded_filename;
	int		fd;
	int		ret;

	filename = node->right->value;
	if (is_ambiguous_redirect(shell, filename))
		return (handle_ambiguous_filename(filename));
	expanded_filename = get_expanded_filename(shell, filename);
	if (!expanded_filename)
		return (1);
	ret = open_output_file(expanded_filename, &fd);
	if (ret != 0)
	{
		free(expanded_filename);
		return (1);
	}
	return (setup_file_descriptor(fd, expanded_filename));
}

int	open_append_file(const char *expanded_filename, int *fd)
{
	*fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (*fd == -1)
	{
		print_error((char *)expanded_filename, "Failed to open file", 1);
		return (1);
	}
	return (0);
}

int	handle_append_redirection(t_ast_node *node, t_shell *shell)
{
	char	*filename;
	char	*expanded_filename;
	int		fd;
	int		ret;

	filename = node->right->value;
	if (is_ambiguous_redirect(shell, filename))
		return (handle_ambiguous_filename(filename));
	expanded_filename = get_expanded_filename(shell, filename);
	if (!expanded_filename)
		return (1);
	ret = open_append_file(expanded_filename, &fd);
	if (ret != 0)
	{
		free(expanded_filename);
		return (1);
	}
	return (setup_file_descriptor(fd, expanded_filename));
}
