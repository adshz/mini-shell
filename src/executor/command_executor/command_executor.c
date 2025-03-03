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

static void	debug_print(t_shell *shell, const char *msg, const char *str)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	ft_putstr_fd(msg, 2);
	if (str)
	{
		ft_putstr_fd("[", 2);
		ft_putstr_fd(str, 2);
		ft_putstr_fd("]", 2);
	}
	ft_putstr_fd("\n", 2);
}

int	execute_command_node(t_shell *shell, t_ast_node *node, bool is_pipe)
{
	int	exec_status;

	if (!node->expanded_argv)
	{
		debug_print(shell, "DEBUG: No expanded argv", NULL);
		exec_status = check_redirection(node);
		return (reset_stds(shell, is_pipe), (exec_status && ERRNO_GENERAL));
	}
	debug_print(shell, "DEBUG: Command to execute: ", node->expanded_argv[0]);
	if (is_builtin((node->expanded_argv)[0]))
	{
		debug_print(shell, "DEBUG: Executing builtin command", NULL);
		exec_status = check_redirection(node);
		if (exec_status != ERRNO_NONE)
			return (reset_stds(shell, is_pipe), ERRNO_GENERAL);
		exec_status = execute_builtin(shell, node->expanded_argv);
		return (reset_stds(shell, is_pipe), exec_status);
	}
	else
	{
		debug_print(shell, "DEBUG: Executing external command", NULL);
		return (execute_external_cmd(shell, node));
	}
	return (ERRNO_GENERAL);
}
