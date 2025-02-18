/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor_signals.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 09:10:39 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 09:20:56 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

// WIFEXITED(status) equivalent: checks if process exited normally
int	exited_normally(int status)
{
	return (((status) & 0x7f) == 0);
}

// WTERMSIG(status) equivalent: gets the signal number of the process
int	get_signal_from_status(int status)
{
	return (status & 0x7f);
}

// WEXITSTATUS(status) equivalent: gets the exit status of the process
int	get_exit_status(int status)
{
	return ((status & 0xff00) >> 8);
}

// WIFSIGNALED(status) equivalent: checks if process was terminated by a signal
int	was_signaled(int status)
{
	return (((status) & 0x7f) != 0);
}
