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

static void cleanup_line_and_tokens(t_shell *shell)
{
    if (shell->tokens)
    {
        free_tokens(shell->tokens);
        shell->tokens = NULL;
    }
    if (shell->line)
    {
        free(shell->line);
        shell->line = NULL;
    }
}

static void cleanup_ast(t_shell *shell)
{
    if (shell->ast)
    {
        free_ast(shell->ast);
        shell->ast = NULL;
    }
}

static void cleanup_env_and_cmds(t_shell *shell)
{
    if (shell->cmds)
    {
        ft_lstclear(&shell->cmds, &free_cmd);
        shell->cmds = NULL;
    }
    if (shell->env)
    {
        hashmap_destroy(shell->env);
        shell->env = NULL;
    }
}

static void cleanup_pids_and_pwd(t_shell *shell)
{
    if (shell->pids)
    {
        free(shell->pids);
        shell->pids = NULL;
    }
    if (shell->old_pwd)
    {
        free(shell->old_pwd);
        shell->old_pwd = NULL;
    }
}

static void cleanup_history(t_shell *shell)
{
    if (shell->history)
    {
        char **tmp = shell->history;
        while (*tmp)
            free(*tmp++);
        free(shell->history);
        shell->history = NULL;
    }
}

static void cleanup_fds(t_shell *shell)
{
    if (shell->stdin_backup > 2)
        close(shell->stdin_backup);
    if (shell->stdout_backup > 2)
        close(shell->stdout_backup);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->term_settings) == -1)
        print_error("tcsetattr", "Failed to restore terminal settings", 1);
}

void cleanup_shell(t_shell *shell)
{
    if (!shell)
        return;
    tcgetattr(STDIN_FILENO, &shell->term_settings);
    cleanup_ast(shell);
    cleanup_line_and_tokens(shell);
    cleanup_env_and_cmds(shell);
    cleanup_pids_and_pwd(shell);
    cleanup_history(shell);
    cleanup_fds(shell);
}

