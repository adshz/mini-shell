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
#include "errors.h"
#include "utils.h"
#include "builtins.h"
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>

static void	handle_child_error(const char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": Command not found", STDERR_FILENO);
	exit(127);
}

static int	check_command(t_shell *shell, t_ast_node *cmd)
{
	if (!cmd || !cmd->value)
		return (0);
	if (is_builtin(cmd->value))
		return (1);
	if (get_command_path(cmd->value, shell->env))
		return (1);
	return (0);
}

static void	execute_left_child(t_shell *shell, t_ast_node *node, int *pfds)
{
	if (!node->left)
		exit(1);

	close(pfds[0]);
	if (dup2(pfds[1], STDOUT_FILENO) == -1)
	{
		close(pfds[1]);
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		exit(1);
	}
	close(pfds[1]);

	if (!check_command(shell, node->left))
		handle_child_error(node->left->value);
	exit(execute_ast(shell, node->left));
}

static void	execute_right_child(t_shell *shell, t_ast_node *node, int *pfds)
{
	if (!node->right)
		exit(1);

	close(pfds[1]);
	if (dup2(pfds[0], STDIN_FILENO) == -1)
	{
		close(pfds[0]);
		ft_putstr_fd("minishell: pipe error\n", STDERR_FILENO);
		exit(1);
	}
	close(pfds[0]);

	if (!check_command(shell, node->right))
		handle_child_error(node->right->value);
	exit(execute_ast(shell, node->right));
}

int	execute_pipe(t_shell *shell, t_ast_node *node)
{
	int		pfds[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status_left;
	int		status_right;
	int     exit_status;

	if (pipe(pfds) == -1)
		return (print_error(NULL, strerror(errno), 1));

	shell->signint_child = true;
	pid_left = fork();
	if (pid_left == -1)
	{
		close(pfds[0]);
		close(pfds[1]);
		return (print_error(NULL, strerror(errno), 1));
	}

	if (pid_left == 0)
		execute_left_child(shell, node, pfds);

	pid_right = fork();
	if (pid_right == -1)
	{
		close(pfds[0]);
		close(pfds[1]);
		kill(pid_left, SIGTERM);
		waitpid(pid_left, NULL, 0);
		return (print_error(NULL, strerror(errno), 1));
	}

	if (pid_right == 0)
		execute_right_child(shell, node, pfds);

	close(pfds[0]);
	close(pfds[1]);

	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	shell->signint_child = false;

	if (WIFEXITED(status_right))
	{
		exit_status = WEXITSTATUS(status_right);
		shell->exit_status = exit_status;  // Update shell's exit status
		return exit_status;
	}
	shell->exit_status = 1;  // Update shell's exit status for error case
	return 1;
}


