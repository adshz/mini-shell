/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:10:55 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 11:55:07 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander/expander.h"

int	handle_variable_expansion(t_shell *shell, t_ast_node *node)
{
	char	*dollar_pos;

	ft_putstr_fd("\nDEBUG: Starting variable expansion\n", STDERR_FILENO);
	if (!node || !node->args)
	{
		ft_putstr_fd("DEBUG: Invalid node or args\n", STDERR_FILENO);
		return (1);
	}
	dollar_pos = ft_strchr(node->args[0], '$');
	if (dollar_pos)
		return (expand_command_name_with_var(shell, node, dollar_pos));
	return (process_remaining_args(shell, node));
}

int	expand_command_args(t_shell *shell, t_ast_node *node)
{
	ft_putstr_fd("\nDEBUG: expand_command_args called\n", STDERR_FILENO);
	if (!node || !node->args || !node->args[0])
	{
		ft_putstr_fd("DEBUG: Invalid node or args\n", STDERR_FILENO);
		return (1);
	}
	ft_putstr_fd("DEBUG: First arg: [", STDERR_FILENO);
	ft_putstr_fd(node->args[0], STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (handle_variable_expansion(shell, node));
}
