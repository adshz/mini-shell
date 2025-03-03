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
	bool	heredoc_interrupted;
	
	heredoc_interrupted = traverse_expand_tree(shell, shell->ast);
	if (heredoc_interrupted || g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED)
	{
		g_signal_status = SHELL_STATE_READLINE;
		return ;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	shell->exit_status = execute_ast_node(shell, shell->ast, false);
}
