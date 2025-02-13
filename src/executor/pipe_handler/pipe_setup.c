/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_setup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:19:23 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:20:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "errors.h"

int	setup_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe failed", 1));
	return (0);
}

int	handle_fork_error(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	return (print_error(NULL, "fork failed", 1));
}
