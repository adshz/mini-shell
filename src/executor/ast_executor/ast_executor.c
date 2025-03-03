/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:24:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:40:11 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_ast_node(t_shell *shell, t_ast_node *ast_tree, bool is_pipe)
{
	config_execution_signals();
	if (!ast_tree)
		return (ERRNO_EMPTY_COMMAND);
	if (ast_tree->type == NODE_PIPE)
		return (execute_pipeline(shell, ast_tree));
	else if (ast_tree->type == NODE_AND)
		return (execute_and_node(shell, ast_tree));
	else if (ast_tree->type == NODE_OR)
		return (execute_or_node(shell, ast_tree));
	else
		return (execute_command_node(shell, shell->ast, is_pipe));
}
