/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_signal_handler.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:22:51 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:22:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	handle_signal_interrupt(t_shell *shell)
{
	if (g_signal_status == SIGINT)
	{
		shell->signint_child = false;
		shell->in_heredoc = false;
		shell->exit_status = 130;
		g_signal_status = SIG_NONE;
		return (130);
	}
	return (0);
}

int	handle_redirection_parent_process(t_shell *shell, int status)
{
	if (exited_normally(status))
	{
		if (g_signal_status == SIG_HEREDOC_INT || shell->heredoc_sigint)
		{
			shell->signal = 0;
			shell->heredoc_sigint = false;
			g_signal_status = SIG_NONE;
			shell->exit_status = 130;
			return (130);
		}
		return (get_exit_status(status));
	}
	if (was_signaled(status))
	{
		shell->exit_status = 128 + get_signal_from_status(status);
		return (shell->exit_status);
	}
	return (1);
}
