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

	if (!node || !node->args)
		return (1);

	ft_printf("\nDEBUG [var_expand]: Processing command arguments\n");
	ft_printf("DEBUG [var_expand]: First argument is: [%s]\n", node->args[0]);
	
	// Handle command name (first argument) specially
	dollar_pos = ft_strchr(node->args[0], '$');
	if (dollar_pos)
	{
		ft_printf("DEBUG [var_expand]: Found $ in command name at position: [%s]\n", dollar_pos);
		ft_printf("DEBUG [var_expand]: Calling expand_command_name_with_var\n");
		int ret = expand_command_name_with_var(shell, node, dollar_pos);
		ft_printf("DEBUG [var_expand]: expand_command_name_with_var returned: %d\n", ret);
		ft_printf("DEBUG [var_expand]: Command name is now: [%s]\n", node->args[0]);
		return (ret);
		// return (expand_command_name_with_var(shell, node, dollar_pos));
	}

	ft_printf("DEBUG [var_expand]: No $ found in command name, processing remaining args\n");
	return (process_remaining_args(shell, node));
}

int	expand_command_args(t_shell *shell, t_ast_node *node)
{
	if (!node || !node->args || !node->args[0])
		return (1);
	ft_printf("\nDEBUG [var_expand]: Starting command args expansion\n");
	return (handle_variable_expansion(shell, node));
}
