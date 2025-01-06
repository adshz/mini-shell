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

static void debug_msg(const char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));
}

static void debug_msg_with_fd(const char *prefix, int fd, const char *suffix)
{
	char buf[32];
	int len = ft_strlen(prefix);
	ft_memcpy(buf, prefix, len);
	char num[12];
	int numlen = 0;
	int tmpfd = fd;
	if (tmpfd == 0)
		num[numlen++] = '0';
	else
	{
		while (tmpfd > 0)
		{
			num[numlen++] = '0' + (tmpfd % 10);
			tmpfd /= 10;
		}
	}
	int i = 0;
	while (--numlen >= 0)
		buf[len++] = num[i++];
	ft_memcpy(buf + len, suffix, ft_strlen(suffix));
	len += ft_strlen(suffix);
	write(STDERR_FILENO, buf, len);
}

static void	execute_left_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	debug_msg("\n=== LEFT CHILD PROCESS START ===\n");
	debug_msg("Initial file descriptors - ");
	debug_msg_with_fd("stdin: ", STDIN_FILENO, ", ");
	debug_msg_with_fd("stdout: ", STDOUT_FILENO, ", ");
	debug_msg_with_fd("pipe[0]: ", pipe_fd[0], ", ");
	debug_msg_with_fd("pipe[1]: ", pipe_fd[1], "\n");

	// Close read end of pipe
	debug_msg("Left child - closing read end of pipe (fd: ");
	debug_msg_with_fd("", pipe_fd[0], ")\n");
	close(pipe_fd[0]);

	// Set up pipe write end as stdout
	debug_msg("Left child - setting up pipe write end as stdout\n");
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		debug_msg("Left child - dup2 failed for stdout\n");
		exit(1);
	}
	close(pipe_fd[1]);

	// Handle redirections
	debug_msg("Left child - setting up redirections\n");
	setup_redirections(shell, node->left);

	// Find and validate command node
	t_ast_node *cmd_node = node->left;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;

	if (!cmd_node || !check_command(shell, cmd_node))
	{
		handle_child_error(cmd_node ? cmd_node->value : "");
	}
	
	debug_msg("Left child - executing command: ");
	if (cmd_node->value)
		debug_msg(cmd_node->value);
	debug_msg("\n");

	int ret = execute_ast(shell, node->left);
	
	debug_msg("Left child - command execution completed with status: ");
	char status_str[12];
	ft_itoa_buf(ret, status_str);
	debug_msg(status_str);
	debug_msg("\n=== LEFT CHILD PROCESS END ===\n");
	
	exit(ret);
}

static void	execute_right_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	debug_msg("\n=== RIGHT CHILD PROCESS START ===\n");
	debug_msg("Initial file descriptors - ");
	debug_msg_with_fd("stdin: ", STDIN_FILENO, ", ");
	debug_msg_with_fd("stdout: ", STDOUT_FILENO, ", ");
	debug_msg_with_fd("pipe[0]: ", pipe_fd[0], ", ");
	debug_msg_with_fd("pipe[1]: ", pipe_fd[1], "\n");

	// Close write end of pipe
	debug_msg("Right child - closing write end of pipe (fd: ");
	debug_msg_with_fd("", pipe_fd[1], ")\n");
	close(pipe_fd[1]);

	// Set up pipe read end as stdin
	debug_msg("Right child - setting up pipe read end as stdin\n");
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		debug_msg("Right child - dup2 failed for stdin\n");
		exit(1);
	}
	close(pipe_fd[0]);

	// Handle redirections
	debug_msg("Right child - setting up redirections\n");
	setup_redirections(shell, node->right);

	// Find and validate command node
	t_ast_node *cmd_node = node->right;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;

	if (!cmd_node || !check_command(shell, cmd_node))
	{
		handle_child_error(cmd_node ? cmd_node->value : "");
	}

	debug_msg("Right child - executing command: ");
	if (cmd_node->value)
		debug_msg(cmd_node->value);
	debug_msg("\n");

	int ret = execute_ast(shell, node->right);
	
	debug_msg("Right child - command execution completed with status: ");
	char status_str[12];
	ft_itoa_buf(ret, status_str);
	debug_msg(status_str);
	debug_msg("\n=== RIGHT CHILD PROCESS END ===\n");
	
	exit(ret);
}

int execute_pipe(t_shell *shell, t_ast_node *node)
{
	int pipe_fd[2];
	pid_t left_pid, right_pid;
	int status;
	int left_status = 0, right_status = 0;

	debug_msg("\n=== PIPE EXECUTION START ===\n");
	
	if (pipe(pipe_fd) == -1)
	{
		debug_msg("Failed to create pipe\n");
		return (print_error(NULL, "pipe failed", 1));
	}
	
	debug_msg_with_fd("Created pipe with fds: read_end(", pipe_fd[0], ") ");
	debug_msg_with_fd("write_end(", pipe_fd[1], ")\n");

	// Execute left child
	left_pid = fork();
	if (left_pid == -1)
	{
		debug_msg("Failed to fork left child\n");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (print_error(NULL, "fork failed", 1));
	}

	if (left_pid == 0)
		execute_left_child(shell, node, pipe_fd);

	// Wait for left child first
	debug_msg("Waiting for left child (pid: ");
	char pid_str[12];
	ft_itoa_buf(left_pid, pid_str);
	debug_msg(pid_str);
	debug_msg(")\n");
	
	waitpid(left_pid, &status, 0);
	left_status = WEXITSTATUS(status);
	debug_msg("Left child exited with status: ");
	ft_itoa_buf(left_status, pid_str);
	debug_msg(pid_str);
	debug_msg("\n");

	// Only stop pipeline for fatal errors (like file not found), not for grep's "no matches" (status 1)
	t_ast_node *cmd_node = node->left;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;

	// If left command is not grep and failed, or if it's grep and returned something other than 0 or 1
	if ((cmd_node && ft_strcmp(cmd_node->args[0], "grep") != 0 && left_status != 0) ||
		(cmd_node && ft_strcmp(cmd_node->args[0], "grep") == 0 && left_status > 1))
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		debug_msg("Left command failed with fatal error, skipping right command\n");
		debug_msg("=== PIPE EXECUTION END ===\n");
		return left_status;
	}

	// Execute right child
	right_pid = fork();
	if (right_pid == -1)
	{
		debug_msg("Failed to fork right child\n");
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (print_error(NULL, "fork failed", 1));
	}

	if (right_pid == 0)
		execute_right_child(shell, node, pipe_fd);

	// Parent process
	debug_msg("Parent process closing pipe fds\n");
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	debug_msg("Waiting for right child (pid: ");
	ft_itoa_buf(right_pid, pid_str);
	debug_msg(pid_str);
	debug_msg(")\n");
	
	waitpid(right_pid, &status, 0);
	right_status = WEXITSTATUS(status);
	debug_msg("Right child exited with status: ");
	ft_itoa_buf(right_status, pid_str);
	debug_msg(pid_str);
	debug_msg("\n");

	if (right_status != 0)
		debug_msg("Pipe execution failed\n");
	else
		debug_msg("Pipe execution succeeded\n");
	
	debug_msg("=== PIPE EXECUTION END ===\n");
	return (right_status);
}


