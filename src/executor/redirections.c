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

	ft_putstr_fd("\n[DEBUG] Handling input redirection for file: ", STDERR_FILENO);
	ft_putendl_fd(node->right->value, STDERR_FILENO);

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
	{
		close(saved_stdin);
		return (print_error(node->right->value, strerror(errno), 1));
	}

	ft_putendl_fd("[DEBUG] Successfully opened input file", STDERR_FILENO);

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		close(saved_stdin);
		return (print_error(node->right->value, "dup2 failed", 1));
	}

	close(fd);
	ft_putendl_fd("[DEBUG] Successfully redirected stdin", STDERR_FILENO);
	return (0);
}

static int	handle_output_redirection(t_ast_node *node)
{
	int	fd;
	int saved_stdout;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
		return (print_error(node->right->value, "Failed to save stdout", 1));

	fd = open(node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
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
	close(saved_stdout);
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
	t_ast_node *current;
	int has_input_error;
	int saved_stdin;
	int saved_stdout;
	int heredoc_pipe[2];
	t_ast_node *redir_nodes[100];  // Array to store redirection nodes
	int redir_count;
	int heredoc_count;
	int i;
	int rightmost_output_index;

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
			ft_putstr_fd("[DEBUG] Found redirection node type: ", STDERR_FILENO);
			ft_putnbr_fd(current->type, STDERR_FILENO);
			ft_putstr_fd(", value: ", STDERR_FILENO);
			ft_putendl_fd(current->right->value, STDERR_FILENO);
		}
		current = current->left;
	}

	// Process heredocs in order
	heredoc_count = 0;
	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type == AST_HEREDOC)
			heredoc_count++;
		i--;
	}

	ft_putstr_fd("[DEBUG] Found heredoc count: ", STDERR_FILENO);
	ft_putnbr_fd(heredoc_count, STDERR_FILENO);
	ft_putchar_fd('\n', STDERR_FILENO);

	// Process all heredocs
	i = redir_count - 1;
	while (i >= 0)
	{
		current = redir_nodes[i];
		if (current->type == AST_HEREDOC)
		{
			// Create pipe for this heredoc
			if (pipe(heredoc_pipe) == -1)
			{
				close(saved_stdin);
				close(saved_stdout);
				exit(1);
			}

			shell->in_heredoc = true;
			ft_putendl_fd("[DEBUG] Entering heredoc mode", STDERR_FILENO);

			char *content = malloc(4096);
			size_t content_size = 0;
			size_t content_capacity = 4096;
			if (!content)
			{
				close(heredoc_pipe[0]);
				close(heredoc_pipe[1]);
				exit(1);
			}

			// Read until delimiter
			while (1)
			{
				// Check for SIGINT first
				if (g_signal == SIGINT)
				{
					ft_putendl_fd("[DEBUG] heredoc: SIGINT received", STDERR_FILENO);
					ft_putstr_fd("[DEBUG] Shell state before cleanup - in_heredoc: ", STDERR_FILENO);
					ft_putnbr_fd(shell->in_heredoc, STDERR_FILENO);
					ft_putstr_fd(", g_signal: ", STDERR_FILENO);
					ft_putnbr_fd(g_signal, STDERR_FILENO);
					ft_putchar_fd('\n', STDERR_FILENO);
					
					free(content);
					shell->in_heredoc = false;
					close(heredoc_pipe[0]);
					close(heredoc_pipe[1]);
					close(saved_stdin);
					close(saved_stdout);
					g_signal = 0;  // Reset signal flag immediately
					
					ft_putstr_fd("[DEBUG] Shell state after cleanup - in_heredoc: ", STDERR_FILENO);
					ft_putnbr_fd(shell->in_heredoc, STDERR_FILENO);
					ft_putstr_fd(", g_signal: ", STDERR_FILENO);
					ft_putnbr_fd(g_signal, STDERR_FILENO);
					ft_putchar_fd('\n', STDERR_FILENO);
					ft_putendl_fd("[DEBUG] heredoc: Exiting with code 130", STDERR_FILENO);
					shell->exit_status = 130;
					exit(130);  // Exit immediately instead of return
				}

				ft_putstr_fd("heredoc> ", STDERR_FILENO);
				char *line = get_next_line(STDIN_FILENO);
				
				if (!line)
				{
					ft_putendl_fd("[DEBUG] heredoc: EOF received", STDERR_FILENO);
					free(content);
					shell->in_heredoc = false;
					close(heredoc_pipe[0]);
					close(heredoc_pipe[1]);
					close(saved_stdin);
					close(saved_stdout);
					ft_putchar_fd('\n', STDERR_FILENO);
					shell->exit_status = 1;  // EOF in heredoc is an error
					exit(1);  // Exit immediately on EOF
				}

				size_t len = ft_strlen(line);
				if (len > 0 && line[len - 1] == '\n')
					line[len - 1] = '\0';

				ft_putstr_fd("[DEBUG] heredoc input: '", STDERR_FILENO);
				ft_putstr_fd(line, STDERR_FILENO);
				ft_putendl_fd("'", STDERR_FILENO);

				if (ft_strcmp(line, current->value) == 0)
				{
					ft_putendl_fd("[DEBUG] heredoc: Delimiter matched", STDERR_FILENO);
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
						close(heredoc_pipe[0]);
						close(heredoc_pipe[1]);
						exit(1);
					}
					ft_strlcpy(new_content, content, content_capacity);
					free(content);
					content = new_content;
					content_capacity *= 2;
				}

				// Append line to content
				ft_strlcpy(content + content_size, line, len);
				content_size += len;
				content[content_size++] = '\n';
				content[content_size] = '\0';
				free(line);
			}

			// Write content to pipe
			write(heredoc_pipe[1], content, content_size);
			free(content);
			close(heredoc_pipe[1]);

			// Set up pipe read end as stdin
			if (dup2(heredoc_pipe[0], STDIN_FILENO) == -1)
			{
				close(heredoc_pipe[0]);
				exit(1);
			}
			close(heredoc_pipe[0]);

			shell->in_heredoc = false;
			ft_putendl_fd("[DEBUG] Exiting heredoc mode", STDERR_FILENO);
		}
		i--;
	}

	// Process input redirections from right to left (last one should take precedence)
	i = redir_count - 1;
	while (i >= 0)
	{
		current = redir_nodes[i];
		if (current->type == AST_REDIR_IN)
		{
			ft_putstr_fd("[DEBUG] Processing input redirection: ", STDERR_FILENO);
			ft_putendl_fd(current->right->value, STDERR_FILENO);
			
			status = handle_input_redirection(current);
			if (status != 0)
			{
				ft_putendl_fd("[DEBUG] Input redirection failed", STDERR_FILENO);
				// Restore original file descriptors before exiting
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		i--;
	}

	// Finally handle output redirections from right to left
	// First find the rightmost output redirection
	rightmost_output_index = -1;
	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type == AST_REDIR_OUT)
		{
			rightmost_output_index = i;
			break;  // Stop at first one we find from right
		}
		i--;
	}

	// First handle all non-rightmost output redirections (just create/truncate)
	i = redir_count - 1;
	while (i >= 0)
	{
		current = redir_nodes[i];
		if (current->type == AST_REDIR_OUT && i != rightmost_output_index)
		{
			int fd;
			fd = open(current->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(1);
			}
			close(fd);
		}
		i--;
	}

	// Then handle append redirections
	i = redir_count - 1;
	while (i >= 0)
	{
		current = redir_nodes[i];
		if (current->type == AST_REDIR_APPEND)
		{
			status = handle_append_redirection(current);
			if (status != 0)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				exit(status);
			}
		}
		i--;
	}

	// Finally handle the rightmost output redirection
	if (rightmost_output_index != -1)
	{
		current = redir_nodes[rightmost_output_index];
		status = handle_output_redirection(current);
		if (status != 0)
		{
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
			exit(status);
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


