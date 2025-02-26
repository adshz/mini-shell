/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:10:55 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:39:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander/expander.h"

int	handle_variable_expansion(t_shell *shell, t_ast_node *node)
{
	char	*dollar_pos;
	int		ret;

	if (!node || !node->args)
		return (1);
	dollar_pos = ft_strchr(node->args[0], '$');
	if (dollar_pos)
	{
		ret = expand_command_name_with_var(shell, node, dollar_pos);
		return (ret);
	}
	return (process_remaining_args(shell, node));
}

int	expand_command_args(t_shell *shell, t_ast_node *node)
{
	if (!node || !node->args || !node->args[0])
		return (1);
	return (handle_variable_expansion(shell, node));
}
