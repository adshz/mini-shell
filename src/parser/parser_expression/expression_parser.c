/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expression.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 07:59:26 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 07:59:51 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

// Main expression parsing function
t_ast_node	*ft_parse_expression(t_shell *shell, int min_prec)
{
	t_ast_node		*left;
	t_ast_node		*right;
	int				n_prec;
	t_token_type	operator;

	if (shell->parse_err.type ||!shell->curr_token)
		return (NULL);
	left = ft_parse_atomic_expression(shell);
	if (!left)
		return (NULL);
	while (is_binary_operator(shell) && curr_token_precd(shell) >= mini_prec)
	{
		operator = shell->curr_token->type;
		ft_get_next_token(shell);
		if (!shell->curr_token)
			return (set_parse_err(E_SYNTAX), left);
		n_prec = ft_precedence(operator) + 1;
		right = ft_parse_expression(shell, n_prec);
		if (!right)
			return (left);
		left = ft_combine_node(operator, left, right);
		if (!left)
			return (ft_cleanup_ast(&left), ft_cleanup_ast(&right));
	}
	return (left);
}

// Get precedence level for a token type
int	ft_precedence(t_token_type type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (0);
	return (1);
}

// Get precedence of current token
int	curr_token_precd(t_shell *shell)
{
	return (ft_precedence(shell->curr_token->type));
}

// Check if current token is a binary operator
bool	is_binary_operator(t_shell *shell)
{
	t_token_type	type;

	if (!shell->curr_token)
		return (false);
	type = shell->curr_token->type;
	if (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR)
		return (true);
	return (false);
}