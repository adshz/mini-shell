/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:31:39 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:32:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Command Node Construction
 *
 * This file builds command nodes in the AST
 */
// Build a command subtree in the AST
t_ast_node	*ft_build_command_subtree(t_shell *shell)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	node = ft_create_new_node(NODE_CMD);
	if (!node)
		return (set_parse_err(E_MEM), NULL);
	while (shell->curr_token && (shell->curr_token->type == TOKEN_IDENTIFIER) \
		|| ft_is_redirection(shell->curr_token->type))
	{
		if (shell->curr_token->type == TOKEN_IDENTIFIER)
		{
			if (!ft_build_raw_command_string(shell, &(node->raw_command)))
				return (clean_cmd_node(node), set_parse_err(E_MEM), NULL);
		}
		else if (ft_is_redirection(shell->curr_token->type))
		{
		if (!ft_process_redirection(shell, &(node->io_list)))
				return (free(node->raw_command), free(node), NULL);
		}
	}
	return (node);
}
