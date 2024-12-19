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
#include "utils.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

static void	execute_pipe_child(t_shell *shell, t_ast_node *node, int pfds[2], int is_left)
{
	if (is_left)
	{
		close(pfds[0]);
		dup2(pfds[1], STDOUT_FILENO);
		close(pfds[1]);
	}
	else
	{
		close(pfds[1]);
		dup2(pfds[0], STDIN_FILENO);
		close(pfds[0]);
	}
	exit(execute_ast(shell, node));
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pfds[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(pfds) == -1)
		return (print_error(NULL, "pipe failed", 1));

	shell->signint_child = true;
	pid_left = fork();
	if (pid_left == -1)
		return (print_error(NULL, "fork failed", 1));
	if (pid_left == 0)
		execute_pipe_child(shell, node->left, pfds, 1);

	pid_right = fork();
	if (pid_right == -1)
		return (print_error(NULL, "fork failed", 1));
	if (pid_right == 0)
		execute_pipe_child(shell, node->right, pfds, 0);

	close(pfds[0]);
	close(pfds[1]);

	waitpid(pid_left, NULL, 0);
	waitpid(pid_right, &status, 0);
	shell->signint_child = false;

	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (WEXITSTATUS(status));
}

void	setup_pipes(int old_pipe[2], int new_pipe[2], int is_first, int is_last)
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

void	close_pipes(int old_pipe[2], int new_pipe[2])
{
	if (old_pipe[0] != -1)
		close(old_pipe[0]);
	if (old_pipe[1] != -1)
		close(old_pipe[1]);
	if (new_pipe[0] != -1)
		close(new_pipe[0]);
	if (new_pipe[1] != -1)
		close(new_pipe[1]);
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
        exit(execute_command(node->left, shell->env));
    }
    else
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        status = execute_command(node->right, shell->env);
        waitpid(pid, NULL, 0);
        return (status);
    }
}


