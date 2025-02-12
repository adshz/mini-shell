#include "libft.h"
#include "framework/test_framework.h"
#include "framework/test_utils.h"
#include "shell_interface.h"
#include "shell.h"
#include "core/core.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>

bool command_name_expansion_test(char **environ)
{
    t_shell     shell;
    bool        success = true;
    int         pipe_fd[2];
    int         saved_stdout;
    char        *output;

    write(STDOUT_FILENO, "\nRunning test: command name expansion\n", 36);
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
    if (saved_stdout == -1)
    {
        write(STDOUT_FILENO, "❌ Failed to duplicate stdout\n", 29);
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        cleanup_shell(&shell);
        return false;
    }

    // Test 1: Basic command expansion
    write(STDOUT_FILENO, "Setting up test environment...\n", 30);
    if (nonint_shell(&shell, "export a='cho 123'") != SHELL_SUCCESS)
    {
        write(STDOUT_FILENO, "❌ Failed to export variable\n", 28);
        success = false;
        goto cleanup;
    }

    write(STDOUT_FILENO, "Redirecting stdout...\n", 21);
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        write(saved_stdout, "❌ Failed to redirect stdout\n", 28);
        success = false;
        goto cleanup;
    }

    write(saved_stdout, "Running command expansion...\n", 28);
    if (nonint_shell(&shell, "e$a") != SHELL_SUCCESS)
    {
        write(saved_stdout, "❌ Failed to execute command\n", 28);
        success = false;
        goto cleanup;
    }

    // Restore stdout before reading from pipe
    close(pipe_fd[1]); // Close write end first
    if (dup2(saved_stdout, STDOUT_FILENO) == -1)
    {
        write(saved_stdout, "❌ Failed to restore stdout\n", 27);
        success = false;
        goto cleanup;
    }

    write(STDOUT_FILENO, "Reading command output...\n", 25);
    output = read_from_pipe(pipe_fd[0]);
    if (!output)
    {
        write(STDOUT_FILENO, "❌ Failed to read output\n", 25);
        success = false;
    }
    else
    {
        const char *expected = "123\n";
        write(STDOUT_FILENO, "Comparing output...\n", 19);
        if (ft_strcmp(output, expected) != 0)
        {
            write(STDOUT_FILENO, "❌ Command expansion output does not match expected\n", 50);
            write(STDOUT_FILENO, "Expected: '", 11);
            write(STDOUT_FILENO, expected, ft_strlen(expected));
            write(STDOUT_FILENO, "'\nGot: '", 8);
            write(STDOUT_FILENO, output, ft_strlen(output));
            write(STDOUT_FILENO, "'\n", 2);
            success = false;
        }
        else
        {
            write(STDOUT_FILENO, "✅ Command expansion output matches expected\n", 44);
        }
        free(output);
        output = NULL;
    }

cleanup:
    write(STDOUT_FILENO, "Cleaning up...\n", 14);
    if (saved_stdout != -1)
    {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
    }
    close(pipe_fd[0]); // Close read end
    cleanup_shell(&shell);
    write(STDOUT_FILENO, "----------------\n", 17);
    return success;
}
