/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:52:27 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:52:27 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor.h"
#include <unistd.h>

int execute_pipe(t_shell *shell, t_ast_node *node)
{
    int old_pipe[2] = {-1, -1};
    int new_pipe[2] = {-1, -1};
    pid_t left_pid, right_pid;
    int status;

    if (pipe(new_pipe) == -1)
        return EXIT_FAILURE;

    // Execute left side of pipe
    left_pid = create_process(shell);
    if (left_pid == 0)
    {
        setup_pipes(old_pipe, new_pipe, 1, 0);
        exit(execute_ast(shell, node->left));
    }

    // Execute right side of pipe
    right_pid = create_process(shell);
    if (right_pid == 0)
    {
        setup_pipes(old_pipe, new_pipe, 0, 1);
        exit(execute_ast(shell, node->right));
    }

    // Parent process
    close_pipes(old_pipe, new_pipe);
    waitpid(left_pid, &status, 0);
    waitpid(right_pid, &status, 0);

    return WEXITSTATUS(status);
}

void setup_pipes(int old_pipe[2], int new_pipe[2], int is_first, int is_last)
{
    if (!is_first)
    {
        dup2(old_pipe[0], STDIN_FILENO);
        close(old_pipe[0]);
        close(old_pipe[1]);
    }

    if (!is_last)
    {
        dup2(new_pipe[1], STDOUT_FILENO);
        close(new_pipe[0]);
        close(new_pipe[1]);
    }
}

void close_pipes(int old_pipe[2], int new_pipe[2])
{
    if (old_pipe[0] != -1) close(old_pipe[0]);
    if (old_pipe[1] != -1) close(old_pipe[1]);
    if (new_pipe[0] != -1) close(new_pipe[0]);
    if (new_pipe[1] != -1) close(new_pipe[1]);
}

int handle_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exit(execute_command(shell, node->left));
    }
    else
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        status = execute_command(shell, node->right);
        waitpid(pid, NULL, 0);
        return (status);
    }
}


