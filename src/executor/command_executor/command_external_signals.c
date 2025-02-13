/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_external_signals.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 09:11:45 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 09:19:58 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

void	setup_child_process(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	setup_parent_process(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	handle_signal_termination(t_shell *shell, int status)
{
	int	sig;

	sig = get_signal_from_status(status);
	if (sig == SIGINT)
	{
		shell->exit_status = 130;
		return (130);
	}
	else if (sig == SIGQUIT)
	{
		if (!shell->in_pipe)
			ft_putendl_fd("Quit: 3", STDERR_FILENO);
		shell->exit_status = 131;
		return (131);
	}
	shell->exit_status = 128 + sig;
	return (128 + sig);
}
