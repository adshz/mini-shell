/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:42:59 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:25:14 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_command_node(t_shell *shell, t_ast_node *node, bool is_pipe)
{
	int	exec_status;

	if (!node->expanded_argv)
	{
		ft_putstr_fd("DEBUG: No expanded argv\n", 2);
		exec_status = check_redirection(node);
		return (reset_stds(shell, is_pipe), (exec_status && ERRNO_GENERAL));
	}
	ft_putstr_fd("DEBUG: Command to execute: [", 2);
	ft_putstr_fd(node->expanded_argv[0], 2);
	ft_putstr_fd("]\n", 2);
	if (is_builtin((node->expanded_argv)[0]))
	{
		ft_putstr_fd("DEBUG: Executing builtin command\n", 2);
		exec_status = check_redirection(node);
		if (exec_status != ERRNO_NONE)
			return (reset_stds(shell, is_pipe), ERRNO_GENERAL);
		exec_status = execute_builtin(shell, node->expanded_argv);
		return (reset_stds(shell, is_pipe), exec_status);
	}
	else
	{
		ft_putstr_fd("DEBUG: Executing external command\n", 2);
		return (execute_external_cmd(shell, node));
	}
	return (ERRNO_GENERAL);
}
