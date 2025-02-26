/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:44:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:45:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_builtin(t_shell *shell, t_ast_node *node)
{
	if (!node || !node->value)
		return (1);
	if (ft_strcmp(node->value, "echo") == 0)
		return (ft_echo(shell, node));
	if (ft_strcmp(node->value, "cd") == 0)
		return (builtin_cd(shell, node));
	if (ft_strcmp(node->value, "pwd") == 0)
		return (builtin_pwd(shell, node));
	if (ft_strcmp(node->value, "export") == 0)
		return (builtin_export(shell, node));
	if (ft_strcmp(node->value, "unset") == 0)
		return (builtin_unset(shell, node));
	if (ft_strcmp(node->value, "env") == 0)
		return (builtin_env(shell, node));
	if (ft_strcmp(node->value, "exit") == 0)
		return (builtin_exit(shell, node));
	return (1);
}
