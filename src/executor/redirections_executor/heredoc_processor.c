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

static int	validate_and_init(t_ast_node *node, char **content, \
	size_t *content_size, size_t *content_capacity)
{
	if (!node)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (!node->right)
		return (print_error("heredoc", "invalid delimiter", 1));
	if (!node->right->value)
		return (print_error("heredoc", "invalid delimiter", 1));
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

int	collect_heredoc_content(t_ast_node *node, t_shell *shell)
{
	char				*content;
	size_t				content_size;
	size_t				content_capacity;
	t_heredoc_content	hdc;
	int					result;

	ft_putstr_fd("\n=== Starting Heredoc Collection ===\n", STDERR_FILENO);
	ft_putstr_fd("Node type: ", STDERR_FILENO);
	ft_putnbr_fd(node->type, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (node->right) {
		ft_putstr_fd("Delimiter: [", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
	}

	content = NULL;
	g_signal_status = SIG_HEREDOC_MODE;
	if (validate_and_init(node, &content, &content_size, &content_capacity))
	{
		ft_putstr_fd("Failed to validate and init heredoc\n", STDERR_FILENO);
		g_signal_status = SIG_NONE;
		return (1);
	}

	ft_putstr_fd("Successfully initialized heredoc. Setting up content structure\n", STDERR_FILENO);
	hdc.content = &content;
	hdc.content_size = &content_size;
	hdc.content_capacity = &content_capacity;
	hdc.shell = shell;

	ft_putstr_fd("Starting to read heredoc content\n", STDERR_FILENO);
	result = read_heredoc_content(node, &hdc);
	if (result != 0 || g_signal_status == SIG_HEREDOC_INT)
	{
		ft_putstr_fd("Failed to read heredoc content or interrupted\n", STDERR_FILENO);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;
		return (1);
	}

	ft_putstr_fd("Initializing heredoc data structure\n", STDERR_FILENO);
	if (init_heredoc_data(&node->data, node->right->value) != 0)
	{
		ft_putstr_fd("Failed to initialize heredoc data\n", STDERR_FILENO);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;
		return (1);
	}

	ft_putstr_fd("Opening content file for writing\n", STDERR_FILENO);
	node->data.content_fd = open(node->data.content_path, O_WRONLY);
	if (node->data.content_fd == -1)
	{
		ft_putstr_fd("Failed to open content file\n", STDERR_FILENO);
		cleanup_heredoc_data(&node->data);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;
		return (1);
	}

	ft_putstr_fd("Writing content to file\n", STDERR_FILENO);
	if (write(node->data.content_fd, content, content_size) == -1)
	{
		ft_putstr_fd("Failed to write content to file\n", STDERR_FILENO);
		close(node->data.content_fd);
		cleanup_heredoc_data(&node->data);
		cleanup_heredoc_resources(&hdc);
		g_signal_status = SIG_NONE;
		return (1);
	}

	ft_putstr_fd("Cleaning up\n", STDERR_FILENO);
	close(node->data.content_fd);
	node->data.content_fd = -1;  // Mark as closed
	cleanup_heredoc_resources(&hdc);
	g_signal_status = SIG_NONE;
	ft_putstr_fd("=== Heredoc Collection Complete ===\n\n", STDERR_FILENO);
	return (0);
}

int	setup_heredoc_pipe(t_ast_node *node)
{
	ft_putstr_fd("\n=== Setting up Heredoc Pipe ===\n", STDERR_FILENO);
	ft_putstr_fd("Node type: ", STDERR_FILENO);
	ft_putnbr_fd(node->type, STDERR_FILENO);
	ft_putstr_fd("\nContent path: [", STDERR_FILENO);
	ft_putstr_fd(node->data.content_path, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	int result = setup_pipe_and_write(&node->data);
	
	if (result != 0)
		ft_putstr_fd("Failed to setup pipe\n", STDERR_FILENO);
	else
		ft_putstr_fd("Successfully setup pipe\n", STDERR_FILENO);

	cleanup_heredoc_data(&node->data);
	ft_putstr_fd("=== Heredoc Pipe Setup Complete ===\n\n", STDERR_FILENO);
	return (result);
}

int	handle_heredoc(t_ast_node *node, t_shell *shell)
{
	ft_putstr_fd("\n=== Starting Heredoc Handler ===\n", STDERR_FILENO);
	if (node->left) {
		ft_putstr_fd("Found nested heredoc on left\n", STDERR_FILENO);
		ft_putstr_fd("Left node type: ", STDERR_FILENO);
		ft_putnbr_fd(node->left->type, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}

	int result = collect_heredoc_content(node, shell);
	if (result == 0) {
		ft_putstr_fd("Successfully collected heredoc content, setting up pipe\n", STDERR_FILENO);
		result = setup_heredoc_pipe(node);
	} else {
		ft_putstr_fd("Failed to collect heredoc content\n", STDERR_FILENO);
	}

	ft_putstr_fd("=== Heredoc Handler Complete ===\n\n", STDERR_FILENO);
	return (result);
}
