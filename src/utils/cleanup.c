/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:49:44 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/utils.h"

void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	
	// Clean up current command resources
	if (shell->ast || shell->tokens || shell->line)
		cleanup_current_command(shell);
	
	// Clean up command list and environment
	if (shell->cmds)
	{
		ft_lstclear(&shell->cmds, &free_cmd);
		shell->cmds = NULL;
	}
	
	// Clean up process state (pipes, redirections, etc.)
	cleanup_process_state(shell);
	
	// Clean up environment
	if (shell->env)
	{
		hashmap_destroy(shell->env);
		shell->env = NULL;
	}
	
	// Clean up history
	rl_clear_history();
	if (shell->history)
	{
		free_history(shell->history);
		shell->history = NULL;
	}
	
	// Clean up terminal state and file descriptors
	cleanup_terminal_state(shell);
	
	// Reset file descriptors to their defaults
	if (shell->stdin_backup != STDIN_FILENO)
	{
		close(shell->stdin_backup);
		shell->stdin_backup = STDIN_FILENO;
	}
	if (shell->stdout_backup != STDOUT_FILENO)
	{
		close(shell->stdout_backup);
		shell->stdout_backup = STDOUT_FILENO;
	}
}
