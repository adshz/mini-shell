/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit_status.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:02:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:03:09 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	exec_get_exit_status(int status)
{
	if (was_signaled(status))
		return (128 + get_signal_from_status(status));
	return (get_exit_status(status))
}
