/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logic_node_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:25:26 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:38:25 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_and_node(t_shell *shell, t_ast_node *node)
{
	int	cmd_exit_status;

	cmd_exit_status = execute_ast_node(shell, node->left, false);
	if (cmd_exit_status == ERRNO_NONE)
		return (execute_ast_node(shell, node->right, false));
	return (cmd_exit_status);
}

int	execute_or_node(t_shell *shell, t_ast_node *node)
{
	int	cmd_exit_status;

	cmd_exit_status = execute_ast_node(shell, node->left, false);
	if (cmd_exit_status == ERRNO_NONE)
		return (execute_ast_node(shell, node->right, false));
}
