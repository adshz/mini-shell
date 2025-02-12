/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_input_handler.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:28:24 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:29:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	open_input_file(const char *expanded_filename, int *fd)
{
	*fd = open(expanded_filename, O_RDONLY);
	if (*fd == -1)
	{
		print_error((char *)expanded_filename, "No such file or directory", 1);
		return (1);
	}
	return (0);
}

int	setup_input_descriptor(int fd, char *expanded_filename)
{
	if (dup2(fd, STDIN_FILENO) == -1)
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

int	handle_input_redirection(t_ast_node *node, t_shell *shell)
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
	ret = open_input_file(expanded_filename, &fd);
	if (ret != 0)
	{
		free(expanded_filename);
		return (1);
	}
	return (setup_input_descriptor(fd, expanded_filename));
}
