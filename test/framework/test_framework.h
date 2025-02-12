#ifndef TEST_FRAMEWORK_H
# define TEST_FRAMEWORK_H

# include <stdbool.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdarg.h>  // Add this for va_list
# include "shell.h"  // Fix the include paths
# include "libft.h"
# include "shell_interface.h"  // Add this back

// Test result structure
typedef struct s_test_result {
    bool success;
    char *error_message;
} t_test_result;

// Test case structure
typedef struct s_test_case {
    const char *name;
    bool (*test_fn)(char **environ);  // Test functions need environ
    bool should_run;
} t_test_case;

// Test suite structure
typedef struct s_test_suite {
    const char *name;
    t_test_case *tests;
    int test_count;
} t_test_suite;

// Test utilities
void redirect_stdout(int *saved_stdout, int *pipe_fd);
char *restore_stdout_and_get_output(int saved_stdout, int *pipe_fd);
char *read_test_output(int fd);

// Test output functions
void test_error(const char *message);
void test_error_with_values(const char *label, const char *value);
void test_success(const char *message);

// Shell test utilities
t_shell *setup_test_shell(char **environ);
void teardown_test_shell(t_shell *shell);
char *execute_command_and_capture_output(t_shell *shell, const char *command);

// Test runner functions
void run_test_suite(t_test_suite *suite, char **environ);
void print_test_results(t_test_suite *suite);

// Add to test_framework.h
void debug_print(const char *format, ...);

#endif 