#include "cd_tests.h"
#include "shell_interface.h"

t_shell *g_shell;  // Add global variable definition for tests

bool test_cd_to_root(char **environ)
{
    t_shell *shell = setup_test_shell(environ);
    if (!shell)
        return false;

    bool success = true;
    char *output = NULL;

    // First cd to root
    nonint_shell(shell, "cd ../../../../../..");
    
    // Run pwd to check current directory
    output = execute_command_and_capture_output(shell, "pwd");
    if (!output)
    {
        debug_print("Failed to capture output\n");
        teardown_test_shell(shell);
        return false;
    }

    // Verify output
    if (ft_strcmp(output, "/\n") != 0)
    {
        debug_print("Expected '/\\n', but got '%s'\n", output);
        success = false;
    }

    free(output);  // Free only if we allocated it
    teardown_test_shell(shell);
    return success;
}

bool test_cd_home(char **environ)
{
    t_shell *shell = setup_test_shell(environ);
    if (!shell)
        return false;

    bool success = true;
    char *output = NULL;
    char *expected = NULL;
    char *home_dir;

    // Get home directory for comparison
    home_dir = hashmap_get(shell->env, "HOME");
    if (!home_dir)
    {
        printf("❌ HOME environment variable not set\n");
        teardown_test_shell(shell);
        return false;
    }

    // Print current directory before cd
    char cwd_before[4096];
    if (getcwd(cwd_before, sizeof(cwd_before)))
        printf("Directory before cd: [%s]\n", cwd_before);

    // First cd to home
    printf("Executing command: cd\n");
    nonint_shell(shell, "cd");
    
    // Print current directory after cd
    char cwd_after[4096];
    if (getcwd(cwd_after, sizeof(cwd_after)))
        printf("Directory after cd:  [%s]\n", cwd_after);
    
    printf("Executing command: pwd\n");
    output = execute_command_and_capture_output(shell, "pwd");

    if (!output)
    {
        printf("❌ Failed to capture output\n");
        teardown_test_shell(shell);
        return false;
    }

    // Create expected output (home_dir with newline)
    expected = ft_strjoin(home_dir, "\n");
    if (!expected)
    {
        printf("❌ Failed to create expected output\n");
        free(output);
        teardown_test_shell(shell);
        return false;
    }

    printf("Expected output: [%s]", expected);
    printf("Actual output:   [%s]", output);

    if (ft_strcmp(output, expected) != 0)
    {
        printf("❌ Output does not match expected value\n");
        printf("Expected length: %zu\n", ft_strlen(expected));
        printf("Actual length:   %zu\n", ft_strlen(output));
        success = false;
    }
    else
        printf("✅ Output matches expected value\n");

    free(output);
    free(expected);
    teardown_test_shell(shell);
    return success;
}

bool test_cd_desktop(char **environ)
{
    t_shell *shell = setup_test_shell(environ);
    if (!shell)
        return false;

    bool success = true;
    char *output = NULL;
    char *expected_path = NULL;
    char *home_dir;

    // Get home directory for comparison
    home_dir = hashmap_get(shell->env, "HOME");
    if (!home_dir)
    {
        printf("❌ HOME environment variable not set\n");
        teardown_test_shell(shell);
        return false;
    }

    // Print current directory before cd
    char cwd_before[4096];
    if (getcwd(cwd_before, sizeof(cwd_before)))
        printf("Directory before cd: [%s]\n", cwd_before);

    // First cd to Desktop using tilde expansion
    printf("Executing command: cd ~/Desktop\n");
    nonint_shell(shell, "cd ~/Desktop");
    
    // Print current directory after cd
    char cwd_after[4096];
    if (getcwd(cwd_after, sizeof(cwd_after)))
        printf("Directory after cd:  [%s]\n", cwd_after);
    
    printf("Executing command: pwd\n");
    output = execute_command_and_capture_output(shell, "pwd");

    if (!output)
    {
        printf("❌ Failed to capture output\n");
        teardown_test_shell(shell);
        return false;
    }
 
    // Create expected path (home_dir + /Desktop + newline)
    char temp_path[4096];
    ft_strlcpy(temp_path, home_dir, sizeof(temp_path));
    ft_strlcat(temp_path, "/Desktop", sizeof(temp_path));
    expected_path = ft_strjoin(temp_path, "\n");
  
    if (!expected_path)
    {
        printf("❌ Failed to create expected path\n");
        free(output);
        teardown_test_shell(shell);
        return false;
    }

    printf("Expected output: [%s]", expected_path);
    printf("Actual output:   [%s]", output);

    if (ft_strcmp(output, expected_path) != 0)
    {
        printf("❌ Output does not match expected value\n");
        printf("Expected length: %zu\n", ft_strlen(expected_path));
        printf("Actual length:   %zu\n", ft_strlen(output));
        success = false;
    }
    else
        printf("✅ Output matches expected value\n");

    free(output);
    free(expected_path);
    teardown_test_shell(shell);
    return success;
}

// Register CD tests
t_test_case cd_test_cases[] = {
    {"CD to root", test_cd_to_root, true},
    {"CD to home", test_cd_home, true},
    {"CD to desktop", test_cd_desktop, true},
    {NULL, NULL, false}
};

const int cd_test_count = sizeof(cd_test_cases) / sizeof(t_test_case) - 1; 