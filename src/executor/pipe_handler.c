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

static int check_command(t_shell *shell, t_ast_node *cmd)
{
	if (!cmd || !cmd->value)
		return (0);
	if (is_builtin(cmd->value))
		return (1);
	if (get_command_path(cmd->value, shell->env))
		return (1);
	return (0);
}

static void	execute_left_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	close(pipe_fd[0]);

	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(pipe_fd[1]);

	setup_redirections(shell, node->left);

	t_ast_node *cmd_node = node->left;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;

	if (!cmd_node || !check_command(shell, cmd_node))
		handle_child_error(cmd_node ? cmd_node->value : "");

	int ret = execute_ast(shell, node->left);
	exit(ret);
}

static void	execute_right_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	close(pipe_fd[1]);

	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		exit(1);
	close(pipe_fd[0]);

	setup_redirections(shell, node->right);

	t_ast_node *cmd_node = node->right;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;

	if (!cmd_node || !check_command(shell, cmd_node))
		handle_child_error(cmd_node ? cmd_node->value : "");

	int ret = execute_ast(shell, node->right);
	exit(ret);
}

int execute_pipe(t_shell *shell, t_ast_node *node)
{
	int pipe_fd[2];
	pid_t left_pid, right_pid;
	int status;
	int right_status = 0;

	if (pipe(pipe_fd) == -1)
		return (print_error(NULL, "pipe failed", 1));

	left_pid = fork();
	if (left_pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (print_error(NULL, "fork failed", 1));
	}

	if (left_pid == 0)
		execute_left_child(shell, node, pipe_fd);

	waitpid(left_pid, &status, 0);

	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (print_error(NULL, "fork failed", 1));
	}

	if (right_pid == 0)
		 execute_right_child(shell, node, pipe_fd);

	close(pipe_fd[0]);
	close(pipe_fd[1]);

	waitpid(right_pid, &status, 0);
	right_status = WEXITSTATUS(status);

	return (right_status);
}


