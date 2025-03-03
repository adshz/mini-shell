/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:45:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:07:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_external_cmd(t_shell *shell, t_ast_node *node)
{
	t_path	path_status;
	int		tmp_status;
	int		child_pid;

	g_signal_status = SHELL_STATE_EXECUTION;
	child_pid = fork();
	if (child_pid == 0)
	{
		tmp_status = check_redirection(node);
		if (tmp_status != ERRNO_NONE)
			(cleanup_minishell(shell), exit(ERRNO_GENERAL));
		path_status = get_path(shell, (node->expanded_argv)[0]);
		if (path_status.err.num != ERRNO_NONE)
		{
			tmp_status = exec_print_err(path_status.err);
			(cleanup_minishell(shell), exit(tmp_status));
		}
		if (execve(path_status.path, node->expanded_argv, shell->environ) == -1)
			(cleanup_minishell(shell), exit(1));
	}
	waitpid(child_pid, &tmp_status, 0);
	g_signal_status = SHELL_STATE_NORMAL;
	return (exec_get_exit_status(tmp_status));
}
