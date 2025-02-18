/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expression.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:40:32 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 23:23:54 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer/lexer.h"
#include "expander/expander.h"

/* Part of the function with fixed tabulation */
/**
 * @brief Parses an expression from the token stream.
 *
 * An expression can be:
 * - Simple command: "echo hello"
 * - Pipeline: "ls | grep foo"
 * - Command with redirections: "echo hello > file"
 *
 * Process:
 * 1. Parse pipeline structure
 * 2. Handle any trailing redirections
 *
 * @param tokens Pointer to current position in token stream
 * @return AST node representing the expression, NULL on error
 *
 * @note Advances token pointer during parsing
 * @see parse_pipeline() for pipeline handling
 * @see parse_redirection_construct() for redirection handling
 */
static bool	expand_next_token(t_token *token, t_shell *shell)
{
	char	*expanded;

	if (!token->next || !ft_strchr(token->next->value, '$'))
		return (true);
	expanded = parse_handle_variable_expansion(shell, token->next->value);
	if (!expanded)
		return (false);
	free(token->next->value);
	token->next->value = expanded;
	return (true);
}

static bool	expand_current_token(t_token *token, t_shell *shell)
{
	char	*expanded;

	expanded = parse_handle_variable_expansion(shell, token->value);
	if (!expanded)
		return (false);
	free(token->value);
	token->value = expanded;
	return (true);
}

t_ast_node	*parse_expression(t_token **tokens, t_shell *shell)
{
	t_ast_node	*node;
	char		*dollar_pos;

	if (!tokens || !*tokens)
		return (NULL);
	if (ft_strchr((*tokens)->value, '$'))
	{
		dollar_pos = ft_strchr((*tokens)->value, '$');
		if (dollar_pos > (*tokens)->value)
		{
			if (!expand_next_token(*tokens, shell))
				return (NULL);
		}
		else if (!expand_current_token(*tokens, shell))
			return (NULL);
	}
	node = parse_pipeline(tokens, shell);
	if (!node)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_REDIRECT_IN)
		node = parse_redirection_construct(node, tokens, shell);
	return (node);
}
