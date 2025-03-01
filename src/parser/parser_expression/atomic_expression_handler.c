/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atomic_expression_handler.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:25:21 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:27:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Terminal Expression Parsing
 *
 * This file handles parsing of atomic expressions such as (commands and
 * parenthesizsed expression
 *
 */
// Parse atomic expressions (terminals in the grammar)
t_ast_node	*ft_parse_atomic_expression(t_shell *shell)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	if (is_binary_operator(shell) || shell->curr_token->type == TOKEN_C_PARENT)
		return (set_parse_err(shell, E_SYNTAX), NULL);
	else if (shell->curr_token->type == TOKEN_O_PARENT)
	{
		ft_get_next_token(shell);
		node = ft_parse_expression(shell, 0);
		if (!node)
			return (set_parse_err(E_MEM), NULL);
		if (!shell->curr_token || shell->curr_token->type != TOKEN_C_PARENT)
			return (set_parse_err(E_SYNTAX), node);
		ft_get_next_token(shell);
		return (node);
	}
	else
		return (ft_build_command_subtree(shell));
}