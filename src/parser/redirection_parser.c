/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"

static t_ast_node	*create_redir_node(t_token_type type, const char *target)
{
	t_ast_node	*redir_node;
	t_ast_node	*target_node;

	redir_node = create_ast_node(get_ast_type(type), NULL);
	if (!redir_node)
		return (NULL);
	target_node = create_ast_node(AST_COMMAND, target);
	if (!target_node)
	{
		free_ast(redir_node);
		return (NULL);
	}
	redir_node->right = target_node;
	return (redir_node);
}

t_ast_node	*handle_redirection(t_ast_node *left, t_token **tokens)
{
	t_token_type	redir_type;
	t_ast_node		*redir_node;
	t_token			*current;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	current = *tokens;
	if (!current || current->type != TOKEN_WORD)
	{
		free_ast(left);
		return (NULL);
	}
	redir_node = create_redir_node(redir_type, current->value);
	if (!redir_node)
	{
		free_ast(left);
		return (NULL);
	}
	redir_node->left = left;
	*tokens = current->next;
	return (redir_node);
} 