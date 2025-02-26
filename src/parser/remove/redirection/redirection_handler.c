/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:27:33 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"

int	is_redirection_token(t_token_type type)
{
	return (type == TOKEN_REDIRECT_IN || \
			type == TOKEN_REDIRECT_OUT || \
			type == TOKEN_APPEND || \
			type == TOKEN_HEREDOC);
}

t_ast_node	*process_initial_redirection(t_token **tokens, t_shell *shell)
{
	if (!*tokens)
		return (NULL);
	if (is_redirection_token((*tokens)->type))
		return (parse_redirection_construct(NULL, tokens, shell));
	return (NULL);
}

t_ast_node	*process_final_redirection(t_ast_node *node, \
										t_token **tokens, \
										t_shell *shell)
{
	if (*tokens && is_redirection_token((*tokens)->type))
		return (parse_redirection_construct(node, tokens, shell));
	return (node);
}
