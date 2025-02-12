/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:57:28 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:59:08 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

static int	validate_and_init(t_ast_node *node, char **content, \
	size_t *content_size, size_t *content_capacity)
{
	if (!node)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (!node->right)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (!node->right->value)
		return (print_error("heredoc", "invalid delimiter", 1));
		
	// Debug: Print initial delimiter information
	ft_putstr_fd("\nDEBUG Heredoc Initialization:\n", STDERR_FILENO);
	ft_putstr_fd("Delimiter node type: ", STDERR_FILENO);
	ft_putnbr_fd(node->right->type, STDERR_FILENO);
	ft_putstr_fd("\nDelimiter value: [", STDERR_FILENO);
	ft_putstr_fd(node->right->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	
	*content_size = 0;
	*content_capacity = 4096;
	return (init_heredoc_content(content));
}

int	handle_heredoc(t_ast_node *node, t_shell *shell)
{
	char	*content;
	size_t	content_size;
	size_t	content_capacity;
	t_heredoc_content hdc;
	int		result;

	shell->in_heredoc = 1;
	content = NULL;
	
	if (validate_and_init(node, &content, &content_size, &content_capacity))
	{
		ft_putstr_fd("DEBUG: Validation or initialization failed\n", STDERR_FILENO);
		shell->in_heredoc = 0;
		return (1);
	}
	
	hdc.content = &content;
	hdc.content_size = &content_size;
	hdc.content_capacity = &content_capacity;
	hdc.shell = shell;
	
	result = read_heredoc_content(node, &hdc);
	shell->in_heredoc = 0;
	
	if (result != 0)
	{
		if (content)
			free(content);
		return (1);
	}

	// Initialize heredoc_data with the necessary information
	t_heredoc_data data;
	if (init_heredoc_data(&data, node->right->value) != 0)
	{
		free(content);
		return (1);
	}

	// Write content to the temporary file
	data.content_fd = open(data.content_path, O_WRONLY);
	if (data.content_fd == -1)
	{
		cleanup_heredoc_data(&data);
		free(content);
		return (1);
	}

	if (write(data.content_fd, content, content_size) == -1)
	{
		close(data.content_fd);
		cleanup_heredoc_data(&data);
		free(content);
		return (1);
	}

	close(data.content_fd);
	free(content);

	// Now setup the pipe with the properly initialized data
	result = setup_pipe_and_write(&data);
	cleanup_heredoc_data(&data);
	return (result);
}
