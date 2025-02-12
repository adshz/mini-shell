#include "echo_tests.h"
#include "shell_interface.h"
#include "libft.h"
#include "framework/test_framework.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

static char *read_from_pipe(int fd)
{
    char    buffer[4096];
    char    *output;
    int     bytes_read;

    bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    if (bytes_read < 0)
        return (NULL);
    buffer[bytes_read] = '\0';
    output = ft_strdup(buffer);
    if (!output)
        return (NULL);
    return (output);
}

bool test_echo_spaces(char **environ)
{
    t_shell     shell;
    bool        success = true;
    int         pipe_fd[2];
    int         saved_stdout;
    char        *output;

    printf("\nRunning test: echo spaces\n");
    printf("----------------\n");

    if (init_shell(&shell, NULL, environ) != SHELL_SUCCESS)
    {
        printf("❌ Failed to initialize shell\n");
        return false;
    }

    if (pipe(pipe_fd) == -1)
    {
        printf("❌ Failed to create pipe\n");
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
        printf("❌ Failed to read output\n");
        success = false;
    }
    else
    {
        const char *expected = "    hello world    \n";
        if (ft_strcmp(output, expected) != 0)
        {
            printf("❌ Echo output does not match expected\n");
            printf("Expected: '%s'\n", expected);
            printf("Got: '%s'\n", output);
            success = false;
        }
        else
            printf("✅ Echo output matches expected\n");
        free(output);
    }

    close(pipe_fd[0]);
    cleanup_shell(&shell);
    printf("----------------\n");
    return success;
}