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
#include "shell.h"
#include "expander.h"
#include "types.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_expanded_filename(t_shell *shell, const char *filename)
{
	char *expanded;
	char *unquoted;
	
	// If filename starts with $, it's a variable
	if (filename[0] == '$')
	{
		// Remove the $ prefix
		expanded = expand_variable(shell, filename + 1);
		return expanded;
	}
	
	// If filename is quoted, remove quotes and expand if needed
	if ((filename[0] == '"' || filename[0] == '\'') && 
		filename[ft_strlen(filename) - 1] == filename[0])
	{
		// Remove surrounding quotes
		unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
		if (!unquoted)
			return NULL;
			
		// If unquoted string starts with $, expand it
		if (unquoted[0] == '$')
		{
			expanded = expand_variable(shell, unquoted + 1);
			free(unquoted);
			return expanded;
		}
		return unquoted;
	}
	
	// Otherwise return a copy of the original filename
	return ft_strdup(filename);
}

int is_ambiguous_redirect(t_shell *shell, const char *value)
{
	char *expanded = expand_variable(shell, value);
	if (!expanded)
		return 1;
	
	// Check if expanded value contains spaces
	if (ft_strchr(expanded, ' '))
	{
		free(expanded);
		return 1;
	}
	
	free(expanded);
	return 0;
}

int handle_input_redirection(t_ast_node *node, t_shell *shell)
{
	char *filename = node->right->value;
	char *expanded_filename;
	int fd;

	if (is_ambiguous_redirect(shell, filename))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		return (1);
	}

	expanded_filename = get_expanded_filename(shell, filename);
	if (!expanded_filename)
		return (1);

	fd = open(expanded_filename, O_RDONLY);
	if (fd == -1)
	{
		print_error(expanded_filename, "No such file or directory", 1);
		free(expanded_filename);
		return (1);
	}

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		print_error("dup2", "Failed to duplicate file descriptor", 1);
		close(fd);
		free(expanded_filename);
		return (1);
	}

	close(fd);
	free(expanded_filename);
	return (0);
}

int handle_output_redirection(t_ast_node *node, t_shell *shell)
{
	char *filename = node->right->value;
	char *expanded_filename;
	int fd;

	if (is_ambiguous_redirect(shell, filename))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		return (1);
	}

	expanded_filename = get_expanded_filename(shell, filename);
	if (!expanded_filename)
		return (1);

	fd = open(expanded_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error(expanded_filename, "Failed to open file", 1);
		free(expanded_filename);
		return (1);
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_error("dup2", "Failed to duplicate file descriptor", 1);
		close(fd);
		free(expanded_filename);
		return (1);
	}

	close(fd);
	free(expanded_filename);
	return (0);
}

int handle_append_redirection(t_ast_node *node, t_shell *shell)
{
	char *filename = node->right->value;
	char *expanded_filename;
	int fd;

	if (is_ambiguous_redirect(shell, filename))
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd((char *)filename, STDERR_FILENO);
		ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
		return (1);
	}

	expanded_filename = get_expanded_filename(shell, filename);
	if (!expanded_filename)
		return (1);

	fd = open(expanded_filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error(expanded_filename, "Failed to open file", 1);
		free(expanded_filename);
		return (1);
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		print_error("dup2", "Failed to duplicate file descriptor", 1);
		close(fd);
		free(expanded_filename);
		return (1);
	}

	close(fd);
	free(expanded_filename);
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
	t_ast_node *current;
	int has_input_error;
	int saved_stdin;
	int saved_stdout;
	t_ast_node *redir_nodes[100];  // Array to store redirection nodes
	int redir_count;
	int i;

	ft_putendl_fd("\n[DEBUG] Starting setup_redirections", STDERR_FILENO);

	has_input_error = 0;
	saved_stdin = -1;
	saved_stdout = -1;
	redir_count = 0;

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

	// Process redirections from left to right (reverse the order since we collected right to left)
	for (i = redir_count - 1; i >= 0; i--)
	{
		current = redir_nodes[i];
		
		// Handle each type of redirection
		if (current->type == AST_REDIR_OUT)
		{
			status = handle_output_redirection(current, shell);
			if (status != 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		else if (current->type == AST_REDIR_APPEND)
		{
			status = handle_append_redirection(current, shell);
			if (status != 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		else if (current->type == AST_REDIR_IN)
		{
			status = handle_input_redirection(current, shell);
			if (status != 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		else if (current->type == AST_HEREDOC)
		{
			status = handle_heredoc(current);
			if (status != 0)
			{
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

	ft_putendl_fd("\n[DEBUG] Starting execute_redirection", STDERR_FILENO);
	shell->signint_child = true;
	
	// If SIGINT was received, don't even start the command
	if (g_signal == SIGINT)
	{
		ft_putendl_fd("[DEBUG] SIGINT detected before fork, stopping", STDERR_FILENO);
		shell->signint_child = false;
		shell->in_heredoc = false;
		shell->exit_status = 130;
		return 130;
	}

	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));

	if (pid == 0)
	{
		ft_putendl_fd("[DEBUG] Child process started", STDERR_FILENO);
		setup_redirections(shell, node);
		
		// If we got SIGINT during heredoc or setup_redirections returned early
		if (g_signal == SIGINT || shell->exit_status == 130)
		{
			ft_putendl_fd("[DEBUG] Child process detected SIGINT, exiting", STDERR_FILENO);
			shell->signint_child = false;
			exit(130);
		}
		
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
		{
			ft_putendl_fd("[DEBUG] No command node found, exiting", STDERR_FILENO);
			exit(shell->exit_status);
		}
			
		// Execute command regardless of input redirection status
		ft_putendl_fd("[DEBUG] Executing command node", STDERR_FILENO);
		exit(execute_ast(shell, cmd_node));
	}

	ft_putendl_fd("[DEBUG] Parent waiting for child", STDERR_FILENO);
	waitpid(pid, &status, 0);
	shell->signint_child = false;
	
	// If child exited due to SIGINT in heredoc, stop the entire pipeline
	if (WEXITSTATUS(status) == 130)
	{
		ft_putendl_fd("[DEBUG] Child exited with SIGINT (130)", STDERR_FILENO);
		shell->exit_status = 130;
		shell->in_heredoc = false;  // Make sure we're out of heredoc mode
		g_signal = 0;  // Reset signal flag
		return 130;  // Stop the pipeline
	}
	
	return (WEXITSTATUS(status));
}



