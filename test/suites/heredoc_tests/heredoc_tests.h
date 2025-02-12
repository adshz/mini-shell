#ifndef HEREDOC_TESTS_H
# define HEREDOC_TESTS_H

# include "../../framework/test_framework.h"

// Test cases
bool test_basic_heredoc(char **environ);
bool test_heredoc_with_redirection(char **environ);

// Test case array and count
static t_test_case heredoc_test_cases[] = {
    {"Basic heredoc", test_basic_heredoc, true},
    {"Heredoc with redirection", test_heredoc_with_redirection, true},
    {NULL, NULL, false}  // Sentinel to mark end of test cases
};

static const int heredoc_test_count = sizeof(heredoc_test_cases) / sizeof(heredoc_test_cases[0]) - 1;

#endif 