/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_status_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:15:10 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:15:13 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "parser/parser.h"

int	handle_child_exit_status(int left_status, int right_status)
{
	if (was_signaled(left_status) || was_signaled(right_status))
	{
		if (get_signal_from_status(left_status) == SIGQUIT || \
			get_signal_from_status(right_status) == SIGQUIT)
		{
			if (get_signal_from_status(right_status) == SIGQUIT)
				ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			return (131);
		}
		if (get_signal_from_status(left_status) == SIGINT || \
			get_signal_from_status(right_status) == SIGINT)
			return (130);
	}
	return (get_exit_status(right_status));
}
