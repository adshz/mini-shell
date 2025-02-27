/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/27 10:46:00 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "lexer/lexer.h"
#include "parser/parser.h"
#include "utils/utils.h"

static void	cleanup_command_resources(t_shell *shell)
{
	if (shell->ast || shell->tokens || shell->line)
		cleanup_current_command(shell);
	if (shell->cmds)
	{
		ft_lstclear(&shell->cmds, &free_cmd);
		shell->cmds = NULL;
	}
}

static void	cleanup_environment_resources(t_shell *shell)
{
	cleanup_process_state(shell);
	cleanup_env_cache(shell);
	if (shell->env)
	{
		hashmap_destroy(shell->env);
		shell->env = NULL;
	}
}

static void	cleanup_history_resources(t_shell *shell)
{
	rl_clear_history();
	if (shell->history)
	{
		free_history(shell->history);
		shell->history = NULL;
	}
}

void	cleanup_file_descriptors(t_shell *shell)
{
	if (shell->stdin_backup != STDIN_FILENO)
	{
        // Debug print
        fprintf(stderr, "Closing stdin_backup: %d\n", shell->stdin_backup);
		close(shell->stdin_backup);
		shell->stdin_backup = STDIN_FILENO;
	}
	if (shell->stdout_backup != STDOUT_FILENO)
	{
        // Debug print
        fprintf(stderr, "Closing stdout_backup: %d\n", shell->stdout_backup);
		close(shell->stdout_backup);
		shell->stdout_backup = STDOUT_FILENO;
	}
}
/*
void cleanup_shell(t_shell *shell)
{
    if (!shell)
        return;
    // First handle terminal cleanup which will close FDs
    cleanup_terminal_state(shell);
    // Then handle all other cleanup...
    if (shell->ast)
    {
        free_ast(shell->ast);
        shell->ast = NULL;
    }
    // Close file descriptors safely
    if (shell->stdin_backup != STDIN_FILENO) // If stdin_backup is not the default value
    {
        close(shell->stdin_backup); // Close the file descriptor
        shell->stdin_backup = STDIN_FILENO; // Reset the value to the default
    }
    if (shell->stdout_backup != STDOUT_FILENO)
    {
        close(shell->stdout_backup);
        shell->stdout_backup = STDOUT_FILENO;
    }
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
    // Free environment
    if (shell->env)
    {
        hashmap_destroy(shell->env);
        shell->env = NULL;
    }
    // Clear command history
    if (shell->cmds)
    {
        ft_lstclear(&shell->cmds, free_cmd);
        shell->cmds = NULL;
    }
	if (shell->history)
	{
		free_history(shell->history);
		shell->history = NULL;
	}
    rl_clear_history();
	cleanup_terminal_state(shell);
}
*/
void	cleanup_shell(t_shell *shell)
{
	if (!shell)
		return ;
	cleanup_command_resources(shell);
	cleanup_environment_resources(shell);
	cleanup_history_resources(shell);
	cleanup_terminal_state(shell);
	cleanup_file_descriptors(shell);
}
