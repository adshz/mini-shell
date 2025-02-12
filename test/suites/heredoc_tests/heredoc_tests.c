#include "heredoc_tests.h"
#include "shell_interface.h"
#include "shell.h"
#include "libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include "test_framework.h"

t_shell *g_shell;  // Add global variable definition for tests

bool test_basic_heredoc(char **environ)
{
	t_shell		shell;
	char		*output;
	bool		success;
	int			saved_stdout;
	int			pipe_fd[2];
	int			saved_stdin;
	int			input_pipe[2];
	const char	*input;

	success = true;
	input = "hello\nworld\nEOF\n";
	
	if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
	{
		test_error("Failed to initialize shell");
		return (false);
	}
	g_shell = &shell;  // Set global shell pointer

	if (pipe(input_pipe) == -1)
	{
		test_error("Failed to create input pipe");
		cleanup_shell(&shell);
		return (false);
	}

	if (pipe(pipe_fd) == -1)
	{
		test_error("Failed to create output pipe");
		close(input_pipe[0]);
		close(input_pipe[1]);
		cleanup_shell(&shell);
		return (false);
	}

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	write(input_pipe[1], input, ft_strlen(input));
	close(input_pipe[1]);

	dup2(input_pipe[0], STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);

	nonint_shell(&shell, "cat << EOF");

	close(pipe_fd[1]);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);

	output = read_test_output(pipe_fd[0]);
	if (!output)
	{
		test_error("Failed to read output");
		success = false;
	}
	else
	{
		char *content = output;
		int prompt_count = 0;
		while (*content && prompt_count < 3)
		{
			if (*content == '>' && (content[1] == ' ' || content[1] == '\n'))
			{
				content += 2;
				prompt_count++;
			}
			else
				content++;
		}
		
		size_t len = ft_strlen(content);
		if (len > 0 && content[len - 1] == '\n')
			content[len - 1] = '\0';
		
		if (ft_strcmp(content, "hello\nworld") == 0)
			test_success("Heredoc content matches expected output");
		else
		{
			test_error("Heredoc content does not match");
			test_error_with_values("Expected", "hello\\nworld");
			test_error_with_values("Got", content);
			success = false;
		}
		free(output);
	}

	close(pipe_fd[0]);
	close(input_pipe[0]);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	cleanup_shell(&shell);
	return (success);
}

bool test_heredoc_with_redirection(char **environ)
{
	t_shell     shell;
	bool        success;
	int         pipe_fd[2];
	int         saved_stdin;
	int         input_pipe[2];
	const char  *input;
	
	success = true;
	input = "test content\nhello\n";
	
	if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
	{
		test_error("Failed to initialize shell");
		return (false);
	}
	g_shell = &shell;

	if (pipe(input_pipe) == -1 || pipe(pipe_fd) == -1)
	{
		test_error("Failed to create pipes");
		cleanup_shell(&shell);
		return (false);
	}

	saved_stdin = dup(STDIN_FILENO);
	if (saved_stdin == -1)
	{
		test_error("Failed to duplicate stdin");
		cleanup_shell(&shell);
		return (false);
	}

	write(input_pipe[1], input, ft_strlen(input));
	close(input_pipe[1]);

	if (dup2(input_pipe[0], STDIN_FILENO) == -1)
	{
		test_error("Failed to redirect stdin");
		cleanup_shell(&shell);
		return (false);
	}

	nonint_shell(&shell, "cat << hello > out1");

	int out1_fd = open("out1", O_RDONLY);
	if (out1_fd == -1)
	{
		success = false;
	}
	else
	{
		char *content = read_test_output(out1_fd);
		if (content)
		{
			free(content);
		}
		close(out1_fd);
	}

	close(input_pipe[0]);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	cleanup_shell(&shell);
	unlink("out1");
	return (success);
}

bool test_heredoc_with_multiple_redirections(char **environ)
{
	t_shell     shell;
	bool        success = true;
	int         saved_stdout, pipe_fd[2];
	int         saved_stdin;
	int         input_pipe[2];
	const char  *input = "test content\nhello\n";  // Content for heredoc
	char        *output;
	
    // initialise shell
	if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
	{
		test_error("Failed to initialize shell");
		return false;
	}

	// Create test input file
	int test_fd = open("test_input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (test_fd == -1)
	{
		test_error("Failed to create test input file");
		cleanup_shell(&shell);
		return false;
	}
    // we write "input file content" to the test file 
	write(test_fd, "input file content\n", 18);
	close(test_fd);

	// Setup pipes
	if (pipe(input_pipe) == -1 || pipe(pipe_fd) == -1)
	{
		test_error("Failed to create pipes");
		cleanup_shell(&shell);
		unlink("test_input.txt");
		return false;
	}

    // save stdin and stdout for later restoration
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);

	// Write heredoc content
    // input_pipe[1] is the write end of the input pipe
	write(input_pipe[1], input, ft_strlen(input));
	close(input_pipe[1]);

    // redirect stdin to our input pipe and stdout to our output pipe
    // basically we duplicate the input pipe to stdin so
    // data --> stdin = input_pipe[0]  -> program
    // program --> stdout = pipe_fd[1] -> out1
	dup2(input_pipe[0], STDIN_FILENO);
	dup2(pipe_fd[1], STDOUT_FILENO);

	// Execute command with multiple redirections
	nonint_shell(&shell, "cat << hello < test_input.txt > out1");

	// Cleanup and verify
	close(pipe_fd[1]);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	
    // read the output from the pipe and duplicate it to the output variable
	output = read_test_output(pipe_fd[0]);
	if (!output)
		test_error("Failed to read output");

	// Verify out1 content
	int out1_fd = open("out1", O_RDONLY);
	if (out1_fd != -1)
	{
		char *out1_content = read_test_output(out1_fd);
		if (out1_content)
		{
			if (ft_strstr(out1_content, "input file content"))
				test_success("File redirection worked correctly");
			else
				test_error("Unexpected content in output file");
			free(out1_content);
		}
		close(out1_fd);
	}

	// Cleanup
	if (output)
		free(output);
	close(pipe_fd[0]);
	close(input_pipe[0]);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
	cleanup_shell(&shell);
    
	unlink("test_input.txt");
	unlink("out1");
	
	return (success);
} 