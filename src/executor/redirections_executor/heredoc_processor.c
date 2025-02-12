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

	ft_putstr_fd("\n=== Starting Heredoc Processing ===\n", STDERR_FILENO);
	content = NULL;
	g_signal_status = SIG_HEREDOC_MODE;  // Enter heredoc mode
	ft_putstr_fd("Set g_signal_status to SIG_HEREDOC_MODE\n", STDERR_FILENO);
	(void)shell;  // We don't use shell anymore
	
	if (validate_and_init(node, &content, &content_size, &content_capacity))
	{
		ft_putstr_fd("DEBUG: Validation or initialization failed\n", STDERR_FILENO);
		g_signal_status = SIG_NONE;  // Exit heredoc mode
		return (1);
	}
	
	hdc.content = &content;
	hdc.content_size = &content_size;
	hdc.content_capacity = &content_capacity;
	hdc.shell = shell;  // Keep shell pointer for heredoc state
	
	ft_putstr_fd("Starting to read heredoc content...\n", STDERR_FILENO);
	result = read_heredoc_content(node, &hdc);
	ft_putstr_fd("Finished reading heredoc content, result: ", STDERR_FILENO);
	ft_putnbr_fd(result, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	
	// Check for interruption or error
	if (result != 0 || g_signal_status == SIG_HEREDOC_INT)
	{
		ft_putstr_fd("Heredoc was interrupted or had an error\n", STDERR_FILENO);
		g_signal_status = SIG_NONE;  // Reset signal state
		return (1);
	}

	ft_putstr_fd("Initializing heredoc data...\n", STDERR_FILENO);
	// Initialize heredoc_data with the necessary information
	t_heredoc_data data;
	if (init_heredoc_data(&data, node->right->value) != 0)
	{
		ft_putstr_fd("Failed to initialize heredoc data\n", STDERR_FILENO);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;  // Reset signal state
		return (1);
	}

	// Write content to the temporary file
	data.content_fd = open(data.content_path, O_WRONLY);
	if (data.content_fd == -1)
	{
		ft_putstr_fd("Failed to open temporary file\n", STDERR_FILENO);
		cleanup_heredoc_data(&data);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;  // Reset signal state
		return (1);
	}

	ft_putstr_fd("Writing content to temporary file...\n", STDERR_FILENO);
	if (write(data.content_fd, content, content_size) == -1)
	{
		ft_putstr_fd("Failed to write content\n", STDERR_FILENO);
		close(data.content_fd);
		cleanup_heredoc_data(&data);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;  // Reset signal state
		return (1);
	}

	close(data.content_fd);

	ft_putstr_fd("Setting up pipe...\n", STDERR_FILENO);
	// Now setup the pipe with the properly initialized data
	result = setup_pipe_and_write(&data);
	cleanup_heredoc_data(&data);
	cleanup_heredoc_resources(&hdc);  // Clean up heredoc resources at the end
	g_signal_status = SIG_NONE;  // Reset signal state
	ft_putstr_fd("=== Heredoc Processing Complete ===\n\n", STDERR_FILENO);
	return (result);
}
