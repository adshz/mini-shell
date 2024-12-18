// tests/test_init.c
#include "unity.h"
#include "../inc/shell.h"
#include "../inc/hashtable.h"
#include "../inc/libft.h"
#include <stdlib.h>
#include <unistd.h>

// Global variables for testing
t_shell *test_shell;
char **test_argv;
char **test_envp;

void setUp(void)
{
    // Allocate and setup test data before each test
    test_shell = malloc(sizeof(t_shell));
    
    // Setup test argv
    test_argv = malloc(sizeof(char*) * 2);
    test_argv[0] = ft_strdup("minishell");
    test_argv[1] = NULL;
    
    // Setup minimal test environment
    test_envp = malloc(sizeof(char*) * 4);
    test_envp[0] = ft_strdup("PATH=/usr/bin:/bin");
    test_envp[1] = ft_strdup("HOME=/home/test");
    test_envp[2] = ft_strdup("USER=testuser");
    test_envp[3] = NULL;
}

void tearDown(void)
{
    // Clean up after each test
    if (test_shell)
    {
        if (test_shell->env)
            hashmap_destroy(test_shell->env);
        if (test_shell->history)
            free(test_shell->history);
        // Close any open file descriptors
        if (test_shell->stdin_backup > 2)
            close(test_shell->stdin_backup);
        if (test_shell->stdout_backup > 2)
            close(test_shell->stdout_backup);
        free(test_shell);
    }
    
    // Free test argv
    if (test_argv)
    {
        free(test_argv[0]);
        free(test_argv);
    }
    
    // Free test environment
    if (test_envp)
    {
        for (int i = 0; test_envp[i]; i++)
            free(test_envp[i]);
        free(test_envp);
    }
}

// tests/test_init.c (continued)

void test_InitShell_ShouldInitializeBasicStructure(void)
{
    // Test basic shell initialization
    init_shell(test_shell, test_argv, test_envp);
    
    // Verify basic structure initialization
    TEST_ASSERT_NOT_NULL(test_shell);
    TEST_ASSERT_EQUAL_INT(0, test_shell->exit_status);
    TEST_ASSERT_NULL(test_shell->cmds);
    TEST_ASSERT_NOT_NULL(test_shell->env);
    TEST_ASSERT_NULL(test_shell->pids);
    TEST_ASSERT_NULL(test_shell->old_pwd);
    TEST_ASSERT_NULL(test_shell->history);
}

void test_InitShell_ShouldSetupFileDescriptors(void)
{
    init_shell(test_shell, test_argv, test_envp);
    
    // Verify file descriptor backups
    TEST_ASSERT_GREATER_THAN(2, test_shell->stdin_backup);
    TEST_ASSERT_GREATER_THAN(2, test_shell->stdout_backup);
}

void test_InitShell_ShouldSetupEnvironment(void)
{
    init_shell(test_shell, test_argv, test_envp);
    
    // Test environment variables
    char *path = hashmap_search(test_shell->env, "PATH");
    TEST_ASSERT_NOT_NULL(path);
    TEST_ASSERT_EQUAL_STRING("/usr/bin:/bin", path);
    
    char *home = hashmap_search(test_shell->env, "HOME");
    TEST_ASSERT_NOT_NULL(home);
    TEST_ASSERT_EQUAL_STRING("/home/test", home);
    
    char *user = hashmap_search(test_shell->env, "USER");
    TEST_ASSERT_NOT_NULL(user);
    TEST_ASSERT_EQUAL_STRING("testuser", user);
}

void test_InitShell_ShouldHandleNullEnvironment(void)
{
    // Free existing test environment
    for (int i = 0; test_envp[i]; i++)
        free(test_envp[i]);
    free(test_envp);
    test_envp = NULL;
    
    init_shell(test_shell, test_argv, NULL);
    
    // Verify minimal environment is created
    TEST_ASSERT_NOT_NULL(test_shell->env);
    TEST_ASSERT_NOT_NULL(hashmap_search(test_shell->env, "PATH"));
    TEST_ASSERT_NOT_NULL(hashmap_search(test_shell->env, "SHLVL"));
}


// tests/test_init.c (continued)

int main(void)
{
    UNITY_BEGIN();
    
    RUN_TEST(test_InitShell_ShouldInitializeBasicStructure);
    RUN_TEST(test_InitShell_ShouldSetupFileDescriptors);
    RUN_TEST(test_InitShell_ShouldSetupEnvironment);
    RUN_TEST(test_InitShell_ShouldHandleNullEnvironment);
    
    return UNITY_END();
}