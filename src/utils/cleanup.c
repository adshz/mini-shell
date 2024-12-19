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
#include "lexer.h"
#include "parser.h"
#include "libft.h"
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>

// Main cleanup function
void cleanup_shell(t_shell *shell)
{
    if (!shell)
        return;
    
    rl_clear_history();
    cleanup_terminal_state(shell);
    
    if (shell->ast || shell->tokens || shell->line)
        cleanup_current_command(shell);
        
    cleanup_env_and_cmds(shell);
    cleanup_process_state(shell);
}