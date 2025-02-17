/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:25 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:03:39 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "export.h"

int	builtin_export(t_shell *shell, t_ast_node *node)
{
	int	i;
	int	status;

	if (!node->args[1])
	{
		hashmap_iterate(shell->env, export_print_env_var);
		return (0);
	}
	status = 0;
	i = 1;
	while (node->args[i])
	{
		if (!export_check_expanded_arg(shell, node->args[i]))
			return (0);
		if (!is_valid_identifier(node->args[i]))
			status = export_handle_invalid_identifier(node->args[i]);
		else
			process_export_argument(shell, node->args[i]);
		i++;
	}
	return (status);
}
