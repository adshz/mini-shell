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
		exec_status = check_redirection(node);
		return (reset_stds(shell, is_pipe), (exec_status && ERRNO_GENERAL));
	}
	else if (is_builtin((node->expanded_argv)[0]))
	{
		exec_status = check_redirection(node);
		if (exec_status != ERRNO_NONE)
			return (reset_stds(shell, is_pipe), ERRNO_GENERAL);
		exec_status = execute_builtin(shell, node->expanded_argv);
		return (reset_stds(shell, is_pipe), tmp_status);
	}
	else
		return (execute_external_cmd(shell, node));
}
