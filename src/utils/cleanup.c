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

static void	cleanup_file_descriptors(t_shell *shell)
{
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
