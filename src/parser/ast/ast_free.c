/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 14:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:00:39 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"

static void	free_command_node(t_ast_node *node)
{
	char	**args;

	if (node->value)
		free(node->value);
	if (node->args)
	{
		args = node->args;
		while (*args)
			free(*args++);
		free(node->args);
	}
}

static void free_redirection_node(t_ast_node *node)
{
    // Free the heredoc data if this is a heredoc node
    if (node->type == AST_HEREDOC) {
        if (node->data.delimiter) {
            free((void*)node->data.delimiter);
            node->data.delimiter = NULL;
        }
        if (node->data.content_path) {
            free(node->data.content_path);
            node->data.content_path = NULL;
        }
        if (node->data.content_fd > 2) {
            close(node->data.content_fd);
            node->data.content_fd = -1;
        }
    }
    free_ast(node->left);
    free_ast(node->right);
    if (node->value)
        free(node->value);
}

static void	free_pipe_node(t_ast_node *node)
{
	free_ast(node->left);
	free_ast(node->right);
}

static bool	is_ast_redirection_node(t_ast_type type)
{
	return (type == AST_REDIR_IN || type == AST_REDIR_OUT || \
			type == AST_REDIR_APPEND || type == AST_HEREDOC);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_PIPE)
		free_pipe_node(node);
	else if (is_ast_redirection_node(node->type))
		free_redirection_node(node);
	else if (node->type == AST_COMMAND)
		free_command_node(node);
	free(node);
}
