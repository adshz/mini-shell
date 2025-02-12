/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:47 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:56:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/utils.h"

// Main cleanup function
void cleanup_shell(t_shell *shell)
{
    if (!shell)
        return;
    
    // First, clear any command-specific resources
    if (shell->ast || shell->tokens || shell->line)
        cleanup_current_command(shell);
    
    // Then clean up command list which might reference env vars
    if (shell->cmds)
    {
        ft_lstclear(&shell->cmds, &free_cmd);
        shell->cmds = NULL;
    }
    
    // Clean up process state (pids, history, etc)
    cleanup_process_state(shell);
    
    // Clean up environment last since other components might reference it
    if (shell->env)
    {
        hashmap_destroy(shell->env);
        shell->env = NULL;
    }
    
    // Finally restore terminal state
    rl_clear_history();
    cleanup_terminal_state(shell);
}