/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:27:22 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:28:25 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* AST Node Creation
 *
 *  This file focuses on creating and configuring AST nodes
 *
 */
// Create a new AST node
t_ast_node	*ft_create_new_node(t_ast_node_type type)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	return (new_node);
}

// Convert token type to AST node type
t_ast_node_type	ft_get_ast_node_type(t_token_type type)
{
	if (type == TOKEN_AND)
		return (NODE_AND);
	if (type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_PIPE);
}

t_ast_node	*ft_combine_node(t_shell *shell, t_token_type operator, \
							t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	node = ft_create_new_node(ft_get_ast_node_type(operator));
	if (!node)
		return (set_parse_err(shell, E_MEM), NULL);
	node->left = left;
	node->right = right;
	return (node);
}
