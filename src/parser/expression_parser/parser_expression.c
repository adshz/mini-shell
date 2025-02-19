/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parser_expression.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/19 18:19:23 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/19 18:19:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "parser/parser.h"

static bool	handle_next_token_expression(t_token *token, t_shell *shell)
{
	char	*expanded;

	expanded = NULL;
	if (!token->next || !ft_strchr(token->next->value, '$'))
		return (true);
	expanded = parse_handle_variable_expansion(shell, token->next->value);
	if (!expanded)
		return (false);
	free(token->next->value);
	token->next->value = expanded;
	return (true);
}

static bool	handle_token_expansion(t_token *token, t_shell *shell)
{
	char	*expanded;

	expanded = NULL;
	expanded = parse_handle_variable_expansion(shell, token->value);
	if (!expanded)
		return (false);
	free(token->value);
	token->value = expanded;
	return (true);
}

/**
 * @brief Expands variables in a token if `$`present.
 * 
 * This function first check if we need to expand, 
 * if not we just build the expression tree 
 * if we need to expand, we check if the variable is 
 * a standalone variable or part of a larger string.
 * 
 * we skip the first tokeen if it has dollar sign in the middle
 * and we expand the next token if it has a dollar sign.
 * @param token The token to expand
 * @param shell The shell instance
 * @return true if the token was expanded, false otherwise
 */
static bool	expand_variable_in_token(t_token *token, t_shell *shell)
{
	char	*dollar_pos;

	if (!ft_strchr(token->value, '$'))
		return (true);
	dollar_pos = ft_strchr(token->value, '$');
	if (dollar_pos > token->value)
		return (handle_next_token_expression(token, shell));
	return (handle_token_expansion(token, shell));
}

static t_ast_node	*build_expression_tree(t_token **tokens, t_shell *shell)
{
	t_ast_node	*node;

	node = NULL;
	node = parse_pipeline(tokens, shell);
	if (!node)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_REDIRECT_IN)
		node = parse_redirection_construct(node, tokens, shell);
	return (node);
}

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
t_ast_node	*parse_expression(t_token **tokens, t_shell *shell)
{
	if (!tokens || !*tokens)
		return (NULL);
	if (expand_variable_in_token(*tokens, shell))
		return (build_expression_tree(tokens, shell));
	return (NULL);
}
