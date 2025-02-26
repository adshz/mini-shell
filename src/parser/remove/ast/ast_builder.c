/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:38:10 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 13:38:24 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

t_ast_node	*create_ast_node(t_ast_type type, char *value)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->value = NULL;
	node->is_expanded = 0;
	node->original = NULL;
	node->data.content_path = NULL;
	node->data.content_fd = -1;
	node->data.delimiter = NULL;
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
		{
			free(node);
			return (NULL);
		}
	}
	return (node);
}

t_ast_type	get_ast_type(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (AST_PIPE);
	if (type == TOKEN_REDIRECT_IN)
		return (AST_REDIR_IN);
	if (type == TOKEN_REDIRECT_OUT)
		return (AST_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (AST_REDIR_APPEND);
	if (type == TOKEN_HEREDOC)
		return (AST_HEREDOC);
	if (type == TOKEN_AND)
		return (AST_AND);
	if (type == TOKEN_OR)
		return (AST_OR);
	return (AST_COMMAND);
}
