/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:29:31 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 09:05:46 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

static int	handle_var_expansion(t_shell *shell, t_ast_node *node)
{
	int	ret;

	ret = handle_variable_assignment(shell, node->value);
	shell->exit_status = ret;
	return (ret);
}

static int	handle_command(t_shell *shell, t_ast_node *node)
{
	int	ret;

	ret = execute_command(shell, node);
	shell->exit_status = ret;
	return (ret);
}

static int	handle_pipe(t_shell *shell, t_ast_node *node)
{
	int	ret;

	ret = execute_pipe(shell, node);
	shell->exit_status = ret;
	return (ret);
}

int	handle_node_by_type(t_shell *shell, t_ast_node *node)
{
	if (AST_VAR_EXPANSION == node->type)
		return (handle_var_expansion(shell, node));
	if (AST_COMMAND == node->type)
		return (handle_command(shell, node));
	if (AST_PIPE == node->type)
		return (handle_pipe(shell, node));
	return (-1);
}

void	execute_ast_node(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return ;
	shell->exit_status = handle_node_by_type(shell, node);
}
