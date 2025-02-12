#include "test_framework.h"
#include "shell_interface.h"
// Test utilities
void redirect_stdout(int *saved_stdout, int *pipe_fd)
{
    *saved_stdout = dup(STDOUT_FILENO);
    pipe(pipe_fd);
    dup2(pipe_fd[1], STDOUT_FILENO);
}

char *read_test_output(int fd)
{
	char	buffer[4096];
	char	*output;
	ssize_t	bytes_read;

	bytes_read = read(fd, buffer, sizeof(buffer) - 1);
	if (bytes_read < 0)
		return (NULL);
	buffer[bytes_read] = '\0';
	output = ft_strdup(buffer);
	return output;  // Will be NULL if ft_strdup fails
}

// Test output functions
void test_error(const char *message)
{
	write(STDOUT_FILENO, "❌ ", 4);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
}

void test_error_with_values(const char *label, const char *value)
{
	write(STDOUT_FILENO, label, ft_strlen(label));
	write(STDOUT_FILENO, ": '", 3);
	write(STDOUT_FILENO, value, ft_strlen(value));
	write(STDOUT_FILENO, "'\n", 2);
}

void test_success(const char *message)
{
	write(STDOUT_FILENO, "✅ ", 4);
	write(STDOUT_FILENO, message, ft_strlen(message));
	write(STDOUT_FILENO, "\n", 1);
}

// Shell test utilities
t_shell *setup_test_shell(char **environ)
{
    t_shell *shell = malloc(sizeof(t_shell));
    if (!shell)
        return NULL;
    
    // Initialize all fields to 0 first
    ft_memset(shell, 0, sizeof(t_shell));
    
    if (init_shell(shell, NULL, environ) != SHELL_SUCCESS)
    {
        free(shell);
        return NULL;
    }
    g_shell = shell;
    return shell;
}

void teardown_test_shell(t_shell *shell)
{
    if (shell)
    {
        cleanup_shell(shell);  // This should handle all internal cleanup
        free(shell);          // Then we free the shell struct itself
    }
}

char *execute_command_and_capture_output(t_shell *shell, const char *command)
{
    int saved_stdout;
    int pipe_fd[2];
    char *output = NULL;

    // Save current stdout
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1)
        return NULL;

    // Create pipe
    if (pipe(pipe_fd) == -1)
    {
        close(saved_stdout);
        return NULL;
    }

    // Redirect stdout to pipe
    if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
    {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(saved_stdout);
        return NULL;
    }

    // Execute command
    nonint_shell(shell, command);

    // Restore stdout
    close(pipe_fd[1]);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    // Read output
    output = read_test_output(pipe_fd[0]);
    close(pipe_fd[0]);

    return output;
}

// Test runner functions
void run_test_suite(t_test_suite *suite, char **environ)
{
    printf("\nRunning test suite: %s\n", suite->name);
    printf("----------------\n\n");

    t_test_case *current_test = suite->tests;
    while (current_test->name != NULL)
    {
        if (current_test->should_run)
        {
            printf("Running test: %s\n", current_test->name);
            printf("----------------\n");
            if (current_test->test_fn(environ))
                printf("✅ %s passed\n", current_test->name);
            else
                printf("❌ %s failed\n", current_test->name);
            printf("----------------\n\n");
        }
        current_test++;
    }
}

void print_test_results(t_test_suite *suite)
{
    int failed = 0;
    int total = 0;

    for (int i = 0; i < suite->test_count; i++)
    {
        if (suite->tests[i].should_run)
            total++;
        else
            failed++;
    }

    printf("\nTest summary: %d test(s) failed out of %d\n", failed, total);
}

void debug_print(const char *format, ...)
{
    char    buffer[4096];
    va_list args;
    int     len;

    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    
    if (len > 0)
        write(STDERR_FILENO, buffer, len);
} 