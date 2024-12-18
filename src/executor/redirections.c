/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:52:37 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:52:37 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor.h"
#include <fcntl.h>
#include <unistd.h>

int execute_redirection(t_shell *shell, t_ast_node *node)
{
    pid_t pid;
    int status;

    pid = create_process(shell);
    if (pid == 0)
    {
        // Child process
        handle_redirections(shell, node);
        exit(execute_ast(shell, node->left));
    }
    else if (pid < 0)
        return EXIT_FAILURE;

    // Parent process
    waitpid(pid, &status, 0);
    return WEXITSTATUS(status);
}

void handle_redirections(t_shell *shell, t_ast_node *node)
{
    (void)shell;

    if (!node)
        return;

    if (node->type == AST_REDIR_IN)
    {
        // ... handle input redirection
    }
    else if (node->type == AST_REDIR_OUT)
    {
        // ... handle output redirection
    }
    else if (node->type == AST_REDIR_APPEND)
    {
        // ... handle append redirection
    }
    else if (node->type == AST_COMMAND || 
             node->type == AST_PIPE || 
             node->type == AST_AND || 
             node->type == AST_OR)
    {
        // Add these cases to handle all enum values
    }
    // ... other cases ...
}


