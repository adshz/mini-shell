#include "libft.h"
#include "parser.h"
#include "shell.h"
#include "errors.h"
#include <fcntl.h>
#include <unistd.h>

static int	handle_input_redirection(t_ast_node *node, t_shell *shell)
{
	int	fd;
	int saved_stdin;
	char *filename = node->right->value;
	char *expanded_filename = NULL;

	// Handle quoted variables
	if ((filename[0] == '"' || filename[0] == '\'') && 
		filename[ft_strlen(filename) - 1] == filename[0])
	{
		char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
		if (!unquoted)
			return (print_error(filename, "Memory allocation failed", 1));

		if (unquoted[0] == '$')
		{
			expanded_filename = expand_variable(shell, unquoted + 1);
			free(unquoted);
			if (!expanded_filename)
				return (print_error(filename, "Variable expansion failed", 1));
			filename = expanded_filename;
		}
		else
		{
			filename = unquoted;
		}
	}
	// Handle unquoted variables
	else if (filename[0] == '$')
	{
		expanded_filename = expand_variable(shell, filename + 1);
		if (!expanded_filename)
			return (print_error(filename, "Variable expansion failed", 1));
		filename = expanded_filename;
	}

	if (is_ambiguous_redirect(shell, node->right->value))
	{
		if (expanded_filename)
			free(expanded_filename);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (1);
	}

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		return (print_error(filename, "Failed to save stdin", 1));
	}

	// Check if file exists first
	if (access(filename, F_OK) == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (1);  // Return error but don't exit
	}

	// Check if file is readable
	if (access(filename, R_OK) == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (1);  // Return error but don't exit
	}
		
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(saved_stdin);
		return (print_error(filename, strerror(errno), 1));
	}

	if (dup2(fd, STDIN_FILENO) == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(fd);
		close(saved_stdin);
		return (print_error(filename, "dup2 failed", 1));
	}

	close(fd);
	if (expanded_filename)
		free(expanded_filename);
	return (0);
}

int is_ambiguous_redirect(t_shell *shell, const char *value)
{
	if (!value)
		return (0);
	
	// If the value is quoted, it's never ambiguous
	if ((value[0] == '"' || value[0] == '\'') && 
		value[ft_strlen(value) - 1] == value[0])
		return (0);
	
	// Handle unquoted variables (e.g., $test)
	if (value[0] == '$')
	{
		char *expanded = expand_variable(shell, value + 1);
		if (!expanded)
			return (0);  // If expansion fails, treat as non-ambiguous
			
		// Check if expanded value contains spaces
		int is_ambiguous = 0;
		char *ptr = expanded;
		while (*ptr)
		{
			if (*ptr == ' ' || *ptr == '\t')
			{
				is_ambiguous = 1;
				break;
			}
			ptr++;
		}
		free(expanded);
		return (is_ambiguous);
	}
	
	// For non-variable values, check directly
	while (*value)
	{
		if (*value == ' ' || *value == '\t')
			return (1);
		value++;
	}
	return (0);
}

static int	handle_output_redirection(t_ast_node *node, t_shell *shell)
{
	int	fd;
	int saved_stdout;
	char *filename = node->right->value;
	char *expanded_filename = NULL;

	// Handle quoted variables
	if ((filename[0] == '"' || filename[0] == '\'') && 
		filename[ft_strlen(filename) - 1] == filename[0])
	{
		char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
		if (!unquoted)
			return (print_error(filename, "Memory allocation failed", 1));

		if (unquoted[0] == '$')
		{
			expanded_filename = expand_variable(shell, unquoted + 1);
			free(unquoted);
			if (!expanded_filename)
				return (print_error(filename, "Variable expansion failed", 1));
			filename = expanded_filename;
		}
		else
		{
			filename = unquoted;
		}
	}
	// Handle unquoted variables
	else if (filename[0] == '$')
	{
		expanded_filename = expand_variable(shell, filename + 1);
		if (!expanded_filename)
			return (print_error(filename, "Variable expansion failed", 1));
		filename = expanded_filename;
	}

	if (is_ambiguous_redirect(shell, node->right->value))
	{
		if (expanded_filename)
			free(expanded_filename);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (1);
	}

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		return (print_error(filename, "Failed to save stdout", 1));
	}

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(saved_stdout);
		return (print_error(filename, "Cannot create file", 1));
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(fd);
		close(saved_stdout);
		return (print_error(filename, "dup2 failed", 1));
	}

	close(fd);
	close(saved_stdout);
	if (expanded_filename)
		free(expanded_filename);
	return (0);
}

int	handle_append_redirection(t_ast_node *node, t_shell *shell)
{
	int	fd;
	int saved_stdout;
	char *filename = node->right->value;
	char *expanded_filename = NULL;

	// Handle quoted variables
	if ((filename[0] == '"' || filename[0] == '\'') && 
		filename[ft_strlen(filename) - 1] == filename[0])
	{
		char *unquoted = ft_substr(filename, 1, ft_strlen(filename) - 2);
		if (!unquoted)
			return (print_error(filename, "Memory allocation failed", 1));

		if (unquoted[0] == '$')
		{
			expanded_filename = expand_variable(shell, unquoted + 1);
			free(unquoted);
			if (!expanded_filename)
				return (print_error(filename, "Variable expansion failed", 1));
			filename = expanded_filename;
		}
		else
		{
			filename = unquoted;
		}
	}
	// Handle unquoted variables
	else if (filename[0] == '$')
	{
		expanded_filename = expand_variable(shell, filename + 1);
		if (!expanded_filename)
			return (print_error(filename, "Variable expansion failed", 1));
		filename = expanded_filename;
	}

	if (is_ambiguous_redirect(shell, node->right->value))
	{
		if (expanded_filename)
			free(expanded_filename);
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(node->right->value, STDERR_FILENO);
		ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
		return (1);
	}

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		return (print_error(filename, "Failed to save stdout", 1));
	}

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(saved_stdout);
		return (print_error(filename, "Cannot create file", 1));
	}

	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		if (expanded_filename)
			free(expanded_filename);
		close(fd);
		close(saved_stdout);
		return (print_error(filename, "dup2 failed", 1));
	}

	close(fd);
	close(saved_stdout);
	if (expanded_filename)
		free(expanded_filename);
	return (0);
} 