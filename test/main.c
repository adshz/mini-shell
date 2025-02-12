#include "framework/test_framework.h"
#include "suites/cd_tests/cd_tests.h"
#include "suites/heredoc_tests/heredoc_tests.h"
#include "suites/echo_tests/echo_tests.h"
#include "suites/command_expansion_tests/command_name_expansions.h"

t_shell *g_shell;  // Add global variable definition for tests

int main(int argc, char **argv, char **environ)
{
    (void)argc;
    (void)argv;

    t_test_suite *current_suite;

    // CD Tests
    current_suite = &(t_test_suite){
        .name = "CD Tests",
        .tests = (t_test_case[]){
            {"CD to root", test_cd_to_root, true},
            {"CD to home", test_cd_home, true},
            {"CD to desktop", test_cd_desktop, true},
            {0}
        }
    };
    run_test_suite(current_suite, environ);

    // Heredoc Tests
    current_suite = &(t_test_suite){
        .name = "Heredoc Tests",
        .tests = (t_test_case[]){
            {"Basic heredoc", test_basic_heredoc, true},
            {"Heredoc with redirection", test_heredoc_with_redirection, true},
            {0}
        },
        .test_count = 2
    };

    // Echo Tests
    current_suite = &(t_test_suite){
        .name = "Echo Tests",
        .tests = (t_test_case[]){
            {"echo_spaces", test_echo_spaces, true},
            {0}
        }
    };

    current_suite = &(t_test_suite){
        .name = "Command Expansion Tests",
        .tests = (t_test_case[]){
            {"command_name_expansion", command_name_expansion_test, true},
            {0}
        }
    };
    run_test_suite(current_suite, environ);

    return 0;
} 