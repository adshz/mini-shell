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
#include "libft.h"
#include "errors.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static int	handle_input_redirection(t_ast_node *node)
{
	int	fd;
	int saved_stdin;

	// Check if file exists first
	if (access(node->right->value, F_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);  // Return error but don't exit
	}

	// Check if file is readable
	if (access(node->right->value, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (1);  // Return error but don't exit
	}

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
		return (print_error(node->right->value, "Failed to save stdin", 1));

	fd = open(node->right->value, O_RDONLY);
	if (fd == -1)
		return (print_error(node->right->value, strerror(errno), 1));

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		close(saved_stdin);
		return (print_error(node->right->value, "dup2 failed", 1));
	}

	close(fd);
	return (0);
}

static int	handle_output_redirection(t_ast_node *node)
{
	int	fd;
	int saved_stdout;

	ft_putstr_fd("DEBUG: Output redirection - to file: ", STDERR_FILENO);
	ft_putstr_fd(node->right->value, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		ft_putstr_fd("DEBUG: Output redirection - failed to save stdout\n", STDERR_FILENO);
		return (print_error(node->right->value, "Failed to save stdout", 1));
	}

	fd = open(node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("DEBUG: Output redirection - failed to open/create file\n", STDERR_FILENO);
		close(saved_stdout);
		return (print_error(node->right->value, "Cannot create file", 1));
	}

	ft_putstr_fd("DEBUG: Output redirection - opened file with fd: ", STDERR_FILENO);
	ft_putnbr_fd(fd, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("DEBUG: Output redirection - dup2 failed\n", STDERR_FILENO);
		close(fd);
		close(saved_stdout);
		return (print_error(node->right->value, "dup2 failed", 1));
	}

	close(fd);
	close(saved_stdout);
	ft_putstr_fd("DEBUG: Output redirection - successfully set up\n", STDERR_FILENO);
	return (0);
}

int	handle_append_redirection(t_ast_node *node)
{
	int	fd;
	int saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	fd = open(node->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		close(saved_stdout);
		return (print_error(node->right->value, "Cannot create file", 1));
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		close(saved_stdout);
		return (print_error(node->right->value, "dup2 failed", 1));
	}
	close(fd);
	return (0);
}

int	handle_heredoc(t_ast_node *node)
{
	int		pipe_fd[2];
	char	*line;
	char	*delimiter;
	char	*content;
	size_t	content_size;
	size_t	content_capacity;

	if (!node || !node->value)
		return (print_error("heredoc", "invalid delimiter", 1));

	delimiter = node->value;
	content = malloc(4096);  // Initial buffer size
	if (!content)
		return (print_error("heredoc", "malloc failed", 1));
	content_size = 0;
	content_capacity = 4096;
	content[0] = '\0';

	// First collect all heredoc content
	while (1)
	{
		ft_putstr_fd("> ", STDOUT_FILENO);
		line = get_next_line(STDIN_FILENO);
		if (!line)
			break;
		
		// Remove newline from the end of line if present
		size_t len = ft_strlen(line);
		if (len > 0 && line[len - 1] == '\n')
			line[len - 1] = '\0';

		// Check if line matches delimiter
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break;
		}

		// Ensure buffer has enough space
		if (content_size + len + 2 > content_capacity)
		{
			char *new_content = malloc(content_capacity * 2);
			if (!new_content)
			{
				free(content);
				free(line);
				return (print_error("heredoc", "malloc failed", 1));
			}
			ft_strlcpy(new_content, content, content_capacity);
			free(content);
			content = new_content;
			content_capacity *= 2;
		}

		// Append line to content
		ft_strlcpy(content + content_size, line, len);
		content_size += len - 1;  // -1 because we removed the newline
		content[content_size++] = '\n';
		content[content_size] = '\0';
		free(line);
	}

	// Now create pipe and write content
	if (pipe(pipe_fd) == -1)
	{
		free(content);
		return (print_error("heredoc", "pipe failed", 1));
	}

	// Write all content at once
	write(pipe_fd[1], content, content_size);
	free(content);
	close(pipe_fd[1]);

	// Set up pipe read end as stdin
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
	{
		close(pipe_fd[0]);
		return (print_error("heredoc", "dup2 failed", 1));
	}
	close(pipe_fd[0]);
	return (0);
}

void	setup_redirections(t_shell *shell, t_ast_node *node)
{
	int status;
	t_ast_node *current = node;
	int has_input_error = 0;
	int saved_stdin = -1;
	int saved_stdout = -1;
	int heredoc_pipe[2] = {-1, -1};
	t_ast_node *redir_nodes[100];  // Array to store redirection nodes
	int redir_count = 0;

	if (!node)
		return;

	// Save original stdin and stdout
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdin == -1 || saved_stdout == -1)
	{
		if (saved_stdin != -1) close(saved_stdin);
		if (saved_stdout != -1) close(saved_stdout);
		exit(1);
	}

	// First collect all redirection nodes from right to left
	current = node;
	while (current && redir_count < 100)
	{
		if (current->type == AST_REDIR_IN || current->type == AST_REDIR_OUT ||
			current->type == AST_REDIR_APPEND || current->type == AST_HEREDOC)
		{
			redir_nodes[redir_count++] = current;
		}
		current = current->left;
	}

	// Process heredocs in order
	for (int i = redir_count - 1; i >= 0; i--)
	{
		current = redir_nodes[i];
		if (current->type == AST_HEREDOC)
		{
			// Close previous heredoc pipe if exists
			if (heredoc_pipe[0] != -1)
			{
				close(heredoc_pipe[0]);
				heredoc_pipe[0] = -1;
			}
			if (heredoc_pipe[1] != -1)
			{
				close(heredoc_pipe[1]);
				heredoc_pipe[1] = -1;
			}

			if (pipe(heredoc_pipe) == -1)
			{
				close(saved_stdin);
				close(saved_stdout);
				exit(1);
			}

			// Write heredoc content
			while (1)
			{
				ft_putstr_fd("heredoc> ", STDERR_FILENO);  // Use STDERR for prompt
				char *line = get_next_line(STDIN_FILENO);  // Read directly from STDIN
				if (!line)
					break;

				size_t len = ft_strlen(line);
				if (len > 0 && line[len - 1] == '\n')
					line[len - 1] = '\0';

				if (ft_strcmp(line, current->value) == 0)
				{
					free(line);
					break;
				}

				// Write the original line with newline
				write(heredoc_pipe[1], line, ft_strlen(line));
				write(heredoc_pipe[1], "\n", 1);
				free(line);
			}

			// Close write end of heredoc pipe
			close(heredoc_pipe[1]);
			heredoc_pipe[1] = -1;

			// Set up read end of heredoc pipe as stdin
			if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
			{
				close(heredoc_pipe[0]);
				close(saved_stdin);
				close(saved_stdout);
				exit(1);
			}
			close(heredoc_pipe[0]);
			heredoc_pipe[0] = -1;
		}
	}

	// Then handle input redirections from left to right
	for (int i = 0; i < redir_count; i++)
	{
		current = redir_nodes[i];
		if (current->type == AST_REDIR_IN)
		{
			status = handle_input_redirection(current);
			if (status != 0)
				has_input_error = 1;
		}
	}

	// Finally handle output redirections from left to right
	for (int i = 0; i < redir_count; i++)
	{
		current = redir_nodes[i];
		if (current->type == AST_REDIR_OUT)
		{
			status = handle_output_redirection(current);
			if (status != 0)
			{
				// Restore original file descriptors before exiting
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		else if (current->type == AST_REDIR_APPEND)
		{
			status = handle_append_redirection(current);
			if (status != 0)
			{
				// Restore original file descriptors before exiting
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
	}

	shell->exit_status = has_input_error;
}

int	execute_redirection(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	shell->signint_child = true;
	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));

	if (pid == 0)
	{
		setup_redirections(shell, node);
		
		// Find the actual command node
		t_ast_node *cmd_node = node;
		while (cmd_node && (cmd_node->type == AST_REDIR_IN || 
			cmd_node->type == AST_REDIR_OUT || 
			cmd_node->type == AST_REDIR_APPEND ||
			cmd_node->type == AST_HEREDOC))
		{
			cmd_node = cmd_node->left;
		}

		// If there's no command (pure redirection), just exit with redirection status
		if (!cmd_node)
			exit(shell->exit_status);
			
		// Execute command regardless of input redirection status
		// This ensures output files are created even if input fails
		exit(execute_ast(shell, cmd_node));
	}

	waitpid(pid, &status, 0);
	shell->signint_child = false;
	return (WEXITSTATUS(status));
}


