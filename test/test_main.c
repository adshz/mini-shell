//
// Created by Michael Lynch on 11/01/2025.
//

#include "shell_interface.h"
#include "shell.h"
#include "libft.h"
#include "core/core.h"
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>

t_shell	*g_shell;

// Add this declaration
void handle_sigint(int sig);

// Test utilities
void redirect_stdout(int *saved_stdout, int *pipe_fd)
{
    *saved_stdout = dup(STDOUT_FILENO);
    pipe(pipe_fd);
    dup2(pipe_fd[1], STDOUT_FILENO);
}

static char *read_from_pipe(int fd)
{
	char	buffer[4096];
	char	*output;
	int		bytes_read;

	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read < 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	output = ft_strdup(buffer);
	if (!output)
		return (NULL);
	return (output);
}
// bool test_basic_heredoc(char **environ)
// {
// 	t_shell		shell;
// 	char		*output;
// 	bool		success;
// 	int			saved_stdout;
// 	int			pipe_fd[2];
// 	int			saved_stdin;
// 	int			input_pipe[2];
// 	const char	*input;

// 	write(STDOUT_FILENO, "\nRunning test: basic heredoc\n", 28);
// 	write(STDOUT_FILENO, "----------------\n", 17);
// 	success = true;
// 	input = "hello\nworld\nEOF\n";
	
// 	if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
// 	{
// 		write(STDOUT_FILENO, "❌ Failed to initialize shell\n", 30);
// 		return (false);
// 	}

// 	if (pipe(input_pipe) == -1)
// 	{
// 		write(STDOUT_FILENO, "❌ Failed to create input pipe\n", 31);
// 		cleanup_shell(&shell);
// 		return (false);
// 	}

// 	if (pipe(pipe_fd) == -1)
// 	{
// 		write(STDOUT_FILENO, "❌ Failed to create output pipe\n", 32);
// 		close(input_pipe[0]);
// 		close(input_pipe[1]);
// 		cleanup_shell(&shell);
// 		return (false);
// 	}

// 	saved_stdin = dup(STDIN_FILENO);
// 	saved_stdout = dup(STDOUT_FILENO);

// 	write(input_pipe[1], input, ft_strlen(input));
// 	close(input_pipe[1]);

// 	dup2(input_pipe[0], STDIN_FILENO);
// 	dup2(pipe_fd[1], STDOUT_FILENO);

// 	nonint_shell(&shell, "cat << EOF");

// 	close(pipe_fd[1]);
//     dup2(saved_stdout, STDOUT_FILENO);
//     close(saved_stdout);
    
// 	output = read_from_pipe(pipe_fd[0]);
//     if (!output)
//     {
// 		write(STDOUT_FILENO, "❌ Failed to read output\n", 25);
// 		success = false;
// 	}
// 	else
// 	{
// 		char *content = output;
// 		int prompt_count = 0;
// 		while (*content && prompt_count < 3)
// 		{
// 			if (*content == '>' && (content[1] == ' ' || content[1] == '\n'))
// 			{
// 				content += 2;
// 				prompt_count++;
// 			}
// 			else
// 				content++;
// 		}
		
// 		size_t len = ft_strlen(content);
// 		if (len > 0 && content[len - 1] == '\n')
// 			content[len - 1] = '\0';
		
// 		if (ft_strcmp(content, "hello\nworld") == 0)
// 			write(STDOUT_FILENO, "✅ Heredoc content matches expected output\n", 43);
// 		else
// 		{
// 			write(STDOUT_FILENO, "❌ Heredoc content does not match\n", 33);
// 			write(STDOUT_FILENO, "Expected: 'hello\\nworld'\n", 24);
// 			write(STDOUT_FILENO, "Got: '", 6);
// 			write(STDOUT_FILENO, content, ft_strlen(content));
// 			write(STDOUT_FILENO, "'\n", 2);
// 			success = false;
// 		}
// 		free(output);
// 	}

// 	close(pipe_fd[0]);
// 	close(input_pipe[0]);
// 	dup2(saved_stdin, STDIN_FILENO);
// 	close(saved_stdin);
// 	cleanup_shell(&shell);
// 	write(STDOUT_FILENO, "----------------\n", 17);
// 	return (success);
// }

// bool test_heredoc_with_redirections(char **environ)
// {
//     t_shell     shell;
//     bool        success;
//     int         saved_stdout;
//     int         pipe_fd[2];
//     int         saved_stdin;
//     int         input_pipe[2];
//     const char  *input;
//     char        *output;
//     char        *out1_content;
//     int         out1_fd;
    
//     write(STDOUT_FILENO, "\nRunning test: heredoc with redirections\n", 39);
//     write(STDOUT_FILENO, "----------------\n", 17);
//     success = true;
//     input = "test content\nhello\n";  // Content + delimiter
    
//     if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
//     {
//         write(STDOUT_FILENO, "❌ Failed to initialize shell\n", 30);
//         return (false);
//     }

//     if (pipe(input_pipe) == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to create input pipe\n", 31);
//         cleanup_shell(&shell);
//         return (false);
//     }

//     if (pipe(pipe_fd) == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to create output pipe\n", 32);
//         close(input_pipe[0]);
//         close(input_pipe[1]);
//         cleanup_shell(&shell);
//         return (false);
//     }

//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);

//     // Write heredoc content to input pipe
//     write(input_pipe[1], input, ft_strlen(input));
//     close(input_pipe[1]);

//     // Redirect stdin to our input pipe and stdout to our output pipe
//     dup2(input_pipe[0], STDIN_FILENO);
//     dup2(pipe_fd[1], STDOUT_FILENO);

//     // Execute the command with redirections
//     nonint_shell(&shell, "<<hello cat <README.md >>out1");

//     // Restore original stdin and stdout
//     close(pipe_fd[1]);
//     dup2(saved_stdout, STDOUT_FILENO);
//     close(saved_stdout);
    
//     // Read command output (if any)
//     output = read_from_pipe(pipe_fd[0]);
//     if (!output)
//     {
//         write(STDOUT_FILENO, "❌ Failed to read output\n", 25);
//         success = false;
//     }

//     // Now verify that out1 contains our heredoc content
//     out1_fd = open("out1", O_RDONLY);
//     if (out1_fd == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to open out1\n", 23);
//         success = false;
//     }
//     else
//     {
//         out1_content = read_from_pipe(out1_fd);
//         if (!out1_content)
//         {
//             write(STDOUT_FILENO, "❌ Failed to read out1\n", 23);
//             success = false;
//         }
//         else
//         {
//             // Skip past any initial prompts
//             char *content = out1_content;
//             while (*content == '>')
//                 content++;
            
//             // Verify the content includes both heredoc and README.md content
//             if (!ft_strstr(content, "test content"))
//             {
//                 write(STDOUT_FILENO, "❌ Heredoc content not found in output\n", 38);
//                 success = false;
//             }
//             else
//                 write(STDOUT_FILENO, "✅ Heredoc content found in output\n", 35);

//             if (!ft_strstr(content, "# norm-shell"))
//             {
//                 write(STDOUT_FILENO, "❌ README.md content not found in output\n", 40);
//                 success = false;
//             }
//             else
//                 write(STDOUT_FILENO, "✅ README.md content found in output\n", 37);

//             free(out1_content);
//         }
//         close(out1_fd);
//     }

//     if (output)
//         free(output);
//     close(pipe_fd[0]);
//     close(input_pipe[0]);
//     dup2(saved_stdin, STDIN_FILENO);
//     close(saved_stdin);
//     cleanup_shell(&shell);
//     unlink("out1");  // Clean up the output file
//     write(STDOUT_FILENO, "----------------\n", 17);
//     return (success);
// }

// bool test_heredoc_redirection(char **environ)
// {
//     t_shell     shell;
//     bool        success;
//     int         saved_stdout;
//     int         pipe_fd[2];
//     int         saved_stdin;
//     int         input_pipe[2];
//     const char  *input;
//     char        *output;
//     char        *out1_content;
//     int         out1_fd;
    
//     write(STDOUT_FILENO, "\nRunning test: heredoc with redirection\n", 39);
//     write(STDOUT_FILENO, "----------------\n", 17);
//     success = true;
//     // We need to wait for the prompt '> ' before each line
//     input = "some test content\n> more content\n> hello\n";
    
//     if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
//     {
//         write(STDOUT_FILENO, "❌ Failed to initialize shell\n", 30);
//         return (false);
//     }

//     if (pipe(input_pipe) == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to create input pipe\n", 31);
//         cleanup_shell(&shell);
//         return (false);
//     }

//     if (pipe(pipe_fd) == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to create output pipe\n", 32);
//         close(input_pipe[0]);
//         close(input_pipe[1]);
//         cleanup_shell(&shell);
//         return (false);
//     }

//     saved_stdin = dup(STDIN_FILENO);
//     saved_stdout = dup(STDOUT_FILENO);

//     // Write heredoc content to input pipe
//     write(input_pipe[1], input, ft_strlen(input));
//     close(input_pipe[1]);

//     // Redirect stdin to our input pipe and stdout to our output pipe
//     dup2(input_pipe[0], STDIN_FILENO);
//     dup2(pipe_fd[1], STDOUT_FILENO);

//     // Execute the command
//     nonint_shell(&shell, "cat << hello > out1");  // Simplified command first

//     // Restore original stdin and stdout
//     close(pipe_fd[1]);
//     dup2(saved_stdout, STDOUT_FILENO);
//     close(saved_stdout);
    
//     // Read command output (if any)
//     output = read_from_pipe(pipe_fd[0]);
//     if (!output)
//     {
//         write(STDOUT_FILENO, "❌ Failed to read output\n", 25);
//         success = false;
//     }

//     // Now verify that out1 contains our heredoc content
//     out1_fd = open("out1", O_RDONLY);
//     if (out1_fd == -1)
//     {
//         write(STDOUT_FILENO, "❌ Failed to open out1\n", 23);
//         success = false;
//     }
//     else
//     {
//         out1_content = read_from_pipe(out1_fd);
//         if (!out1_content)
//         {
//             write(STDOUT_FILENO, "❌ Failed to read out1\n", 23);
//             success = false;
//         }
//         else
//         {
//             // Skip past any initial prompts
//             char *content = out1_content;
//             while (*content == '>')
//                 content++;
            
//             const char *expected = "some test content\n> more content\n";
//             if (ft_strncmp(content, expected, ft_strlen(expected)) != 0)
//             {
//                 write(STDOUT_FILENO, "❌ Heredoc content in out1 does not match expected\n", 49);
//                 write(STDOUT_FILENO, "Expected:\n", 10);
//                 write(STDOUT_FILENO, expected, ft_strlen(expected));
//                 write(STDOUT_FILENO, "\nGot:\n", 6);
//                 write(STDOUT_FILENO, content, ft_strlen(content));
//                 write(STDOUT_FILENO, "\n", 1);
//                 success = false;
//             }
//             else
//                 write(STDOUT_FILENO, "✅ Heredoc content matches expected output\n", 43);
//             free(out1_content);
//         }
//         close(out1_fd);
//     }

//     if (output)
//         free(output);
//     close(pipe_fd[0]);
//     close(input_pipe[0]);
//     dup2(saved_stdin, STDIN_FILENO);
//     close(saved_stdin);
//     cleanup_shell(&shell);
//     unlink("out1");  // Clean up the output file
//     write(STDOUT_FILENO, "----------------\n", 17);
//     return (success);
// }

bool test_echo_spaces(char **environ)
{
    t_shell     shell;
    bool        success = true;
    int         pipe_fd[2];
    int         saved_stdout;
    char        *output;

    write(STDOUT_FILENO, "\nRunning test: echo spaces\n", 26);
    write(STDOUT_FILENO, "----------------\n", 17);

    if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
    {
        write(STDOUT_FILENO, "❌ Failed to initialize shell\n", 30);
        return false;
    }

    if (pipe(pipe_fd) == -1)
    {
        write(STDOUT_FILENO, "❌ Failed to create pipe\n", 25);
        cleanup_shell(&shell);
        return false;
    }

    saved_stdout = dup(STDOUT_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);

    // Test echo with multiple spaces and quotes
    nonint_shell(&shell, "echo ' ' ' ' hello world ' ' ' '");

    close(pipe_fd[1]);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    output = read_from_pipe(pipe_fd[0]);
    if (!output)
    {
        write(STDOUT_FILENO, "❌ Failed to read output\n", 25);
        success = false;
    }
    else
    {
        const char *expected = "    hello world    \n";
        if (ft_strcmp(output, expected) != 0)
        {
            write(STDOUT_FILENO, "❌ Echo output does not match expected\n", 38);
            write(STDOUT_FILENO, "Expected: '", 11);
            write(STDOUT_FILENO, expected, ft_strlen(expected));
            write(STDOUT_FILENO, "'\nGot: '", 8);
            write(STDOUT_FILENO, output, ft_strlen(output));
            write(STDOUT_FILENO, "'\n", 2);
            success = false;
        }
        else
        {
            write(STDOUT_FILENO, "✅ Echo output matches expected\n", 32);
        }
        free(output);
    }

    close(pipe_fd[0]);
    cleanup_shell(&shell);
    write(STDOUT_FILENO, "----------------\n", 17);
    return success;
}

bool test_heredoc_ctrl_c(char **environ)
{
    t_shell     shell;
    bool        success = true;
    int         saved_stdout;
    int         pipe_fd[2];
    int         saved_stdin;
    int         input_pipe[2];
    
    write(STDOUT_FILENO, "\nRunning test: heredoc with Ctrl+C\n", 34);
    write(STDOUT_FILENO, "----------------\n", 17);
    
    // Initialize global shell
    g_shell = malloc(sizeof(t_shell));
    if (!g_shell)
        return (false);
    
    // Initialize shell
    if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
    {
        write(STDOUT_FILENO, "❌ Failed to initialize shell\n", 30);
        free(g_shell);
        return false;
    }
    *g_shell = shell;  // Copy shell data to global shell

    // Create pipes
    if (pipe(input_pipe) == -1)
    {
        write(STDOUT_FILENO, "❌ Failed to create input pipe\n", 31);
        cleanup_shell(&shell);
        free(g_shell);
        return false;
    }

    if (pipe(pipe_fd) == -1)
    {
        write(STDOUT_FILENO, "❌ Failed to create output pipe\n", 32);
        close(input_pipe[0]);
        close(input_pipe[1]);
        cleanup_shell(&shell);
        free(g_shell);
        return false;
    }

    // Save original stdin/stdout
    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);

    // Set up pipe redirection
    dup2(input_pipe[0], STDIN_FILENO);
    dup2(pipe_fd[1], STDOUT_FILENO);

    // Write first line to input pipe
    write(input_pipe[1], "line1\n", 6);
    usleep(10000);  // Give some time for the line to be processed

    // Start heredoc in non-interactive mode
    g_shell->in_heredoc = true;  // Ensure we're in heredoc mode
    shell.in_heredoc = true;
    
    // Fork a child process to handle the heredoc
    pid_t pid = fork();
    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);  // Use default signal handling
        nonint_shell(&shell, "cat << EOF");
        exit(0);
    }
    else if (pid > 0)
    {
        // Parent process
        usleep(50000);  // Wait for heredoc to start
        
        // Write second line and simulate Ctrl+C
        write(input_pipe[1], "line2\n", 6);
        usleep(10000);  // Wait for line to be processed
        
        // Send SIGINT to the child process
        kill(pid, SIGINT);
        
        // Close input pipe
        close(input_pipe[1]);
        
        // Wait for child to finish
        int status;
        waitpid(pid, &status, 0);
        
        // Check if process was terminated by signal
        if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
        {
            write(STDOUT_FILENO, "✅ Heredoc was properly interrupted by SIGINT\n", 45);
        }
        else
        {
            write(STDOUT_FILENO, "❌ Heredoc was not properly interrupted\n", 39);
            success = false;
        }
    }
    else
    {
        write(STDOUT_FILENO, "❌ Failed to fork process\n", 26);
        success = false;
    }

    // Cleanup
    close(pipe_fd[1]);
    close(pipe_fd[0]);
    close(input_pipe[0]);
    dup2(saved_stdin, STDIN_FILENO);
    close(saved_stdin);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
    
    cleanup_shell(&shell);
    free(g_shell);
    write(STDOUT_FILENO, "----------------\n", 17);
    return success;
}

bool test_heredoc_with_multiple_redirections(char **environ)
{
    t_shell shell;
    bool success = true;
    int pipe_fd[2];
    int saved_stdout;
    char *output;

    if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
        return false;

    if (pipe(pipe_fd) == -1)
    {
        cleanup_shell(&shell);
        return false;
    }

    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        cleanup_shell(&shell);
        return false;
    }

    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        success = false;
        goto cleanup;
    }

    if (nonint_shell(&shell, "cat << EOF > test.txt") != SHELL_SUCCESS)
    {
        success = false;
        goto cleanup;
    }

cleanup:
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    cleanup_shell(&shell);
    return success;
}

int main(int argc, char **argv, char **environ)
{
    int failed = 0;
    (void)argc;
    (void)argv;
    
    printf("Running tests...\n\n");

    if (!test_heredoc_ctrl_c(environ))
    {
        printf("❌ test_heredoc_ctrl_c failed\n");
        failed++;
    }
    else
        printf("✅ test_heredoc_ctrl_c passed\n");

    if (!test_command_name_expansion(environ))
    {
        printf("❌ test_command_name_expansion failed\n");
        failed++;
    }
    else
        printf("✅ test_command_name_expansion passed\n");

    printf("\nTest summary: %d test(s) failed\n", failed);
    if (failed)
        return 1;
    return 0;
}
