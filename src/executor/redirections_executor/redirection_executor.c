/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:22:14 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:22:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	execute_redirection(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child_process(shell, node);
	}
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	signal(SIGINT, handle_sigint);
	return (handle_redirection_parent_process(shell, status));
}
