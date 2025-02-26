/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 16:47:58 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:48:16 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

void	execute_shell_command(t_shell *shell)
{
	traverse_tree_heredoc(shell, shell->ast);
	if (g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED)
	{
		ft_cleanup_ast(shell->ast);
		g_signal_status == SHELL_STATE_READLINE;
	}
	termsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	shell->exit_status = execute_ast_node(shell, shell->ast, false);
}
