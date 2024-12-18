#include "unity_config.h"
#include "unity.h"
#include "shell.h"
#include "builtins.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>

// Global variables for testing
static t_shell *shell;
static t_ast_node *node;
static char *test_env[] = {
    "SHLVL=1",
    "PATH=/usr/local/bin:/usr/bin:/bin",
    NULL
};

void setUp(void)
{
    shell = malloc(sizeof(t_shell));
    TEST_ASSERT_NOT_NULL(shell);
    
    char *argv[] = {"minishell", NULL};
    init_shell(shell, argv, test_env);

    // Set up initial environment with platform-independent paths
    char cwd[PATH_MAX];
    TEST_ASSERT_NOT_NULL(getcwd(cwd, sizeof(cwd)));
    hashmap_set(shell->env, "PWD", cwd);
    hashmap_set(shell->env, "HOME", cwd);
    hashmap_set(shell->env, "OLDPWD", cwd);

    node = malloc(sizeof(t_ast_node));
    TEST_ASSERT_NOT_NULL(node);
    node->type = AST_COMMAND;
    node->args = NULL;
}

void tearDown(void)
{
    if (node)
    {
        if (node->args)
        {
            char **tmp = node->args;
            while (*tmp)
                free(*tmp++);
            free(node->args);
        }
        free(node);
    }
    if (shell)
    {
        cleanup_shell(shell);
        free(shell);
    }
}

// PWD Tests
void test_Builtin_PWD_ShouldReturnCurrentDirectory(void)
{
    char *expected = getcwd(NULL, 0);
    TEST_ASSERT_NOT_NULL(expected);
    
    node->args = malloc(2 * sizeof(char *));
    node->args[0] = ft_strdup("pwd");
    node->args[1] = NULL;

    int result = builtin_pwd(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    free(expected);
}

// ECHO Tests
void test_Builtin_Echo_Basic(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = ft_strdup("hello world");
    node->args[2] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Echo_NoArgs(void)
{
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Echo_WithNewlineOption(void)
{
    node->args = malloc(4 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = ft_strdup("-n");
    node->args[2] = ft_strdup("hello world");
    node->args[3] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Echo_MultipleArgs(void)
{
    node->args = malloc(5 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = ft_strdup("hello");
    node->args[2] = ft_strdup("beautiful");
    node->args[3] = ft_strdup("world");
    node->args[4] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Echo_WithMultipleNewlineOptions(void)
{
    node->args = malloc(5 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = ft_strdup("-n");
    node->args[2] = ft_strdup("-n");
    node->args[3] = ft_strdup("hello");
    node->args[4] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Echo_WithInvalidOption(void)
{
    node->args = malloc(4 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("echo");
    node->args[1] = ft_strdup("-x");
    node->args[2] = ft_strdup("hello");
    node->args[3] = NULL;

    int result = builtin_echo(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

// CD Tests
void test_Builtin_CD_ToHome(void)
{
    node->args = malloc(2 * sizeof(char *));
    node->args[0] = ft_strdup("cd");
    node->args[1] = NULL;

    int result = builtin_cd(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);

    char *pwd = hashmap_get(shell->env, "PWD");
    TEST_ASSERT_NOT_NULL(pwd);
    char *home = hashmap_get(shell->env, "HOME");
    TEST_ASSERT_NOT_NULL(home);
    TEST_ASSERT_EQUAL_STRING(home, pwd);
}

void test_Builtin_CD_WithPath(void)
{
    char *test_path = "/tmp";
    char real_path[PATH_MAX];
    
    // Get the real path before cd
    TEST_ASSERT_NOT_NULL(realpath(test_path, real_path));

    node->args = malloc(3 * sizeof(char *));
    node->args[0] = ft_strdup("cd");
    node->args[1] = ft_strdup(test_path);
    node->args[2] = NULL;

    int result = builtin_cd(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);

    char *pwd = hashmap_get(shell->env, "PWD");
    TEST_ASSERT_NOT_NULL(pwd);
    TEST_ASSERT_EQUAL_STRING(real_path, pwd);
}

void test_Builtin_CD_NonexistentPath(void)
{
    node->args = malloc(3 * sizeof(char *));
    node->args[0] = ft_strdup("cd");
    node->args[1] = ft_strdup("/nonexistent_directory_12345");
    node->args[2] = NULL;

    int result = builtin_cd(shell, node);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should fail with non-existent directory
}

void test_Builtin_CD_ToOldPWD(void)
{
    // First change to /tmp
    char *start_dir = getcwd(NULL, 0);
    TEST_ASSERT_NOT_NULL(start_dir);

    node->args = malloc(3 * sizeof(char *));
    node->args[0] = ft_strdup("cd");
    node->args[1] = ft_strdup("/tmp");
    node->args[2] = NULL;

    int result = builtin_cd(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);

    // Then use cd -
    free(node->args[1]);
    node->args[1] = ft_strdup("-");

    result = builtin_cd(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);

    char *pwd = hashmap_get(shell->env, "PWD");
    TEST_ASSERT_NOT_NULL(pwd);
    TEST_ASSERT_EQUAL_STRING(start_dir, pwd);

    free(start_dir);
}

// EXPORT Tests
void test_Builtin_Export_NoArgs(void)
{
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Export_WithValidArg(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = ft_strdup("TEST_VAR=value");
    node->args[2] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    char *value = hashmap_get(shell->env, "TEST_VAR");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("value", value);
}

void test_Builtin_Export_WithMultipleArgs(void)
{
    node->args = malloc(4 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = ft_strdup("VAR1=value1");
    node->args[2] = ft_strdup("VAR2=value2");
    node->args[3] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    char *value1 = hashmap_get(shell->env, "VAR1");
    char *value2 = hashmap_get(shell->env, "VAR2");
    TEST_ASSERT_NOT_NULL(value1);
    TEST_ASSERT_NOT_NULL(value2);
    TEST_ASSERT_EQUAL_STRING("value1", value1);
    TEST_ASSERT_EQUAL_STRING("value2", value2);
}

void test_Builtin_Export_WithInvalidIdentifier(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = ft_strdup("2INVALID=value");
    node->args[2] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should fail with invalid identifier
}

void test_Builtin_Export_WithoutValue(void)
{
    // First set a value
    hashmap_set(shell->env, "TEST_VAR", "old_value");
    
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = ft_strdup("TEST_VAR");
    node->args[2] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    // Value should remain unchanged
    char *value = hashmap_get(shell->env, "TEST_VAR");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("old_value", value);
}

void test_Builtin_Export_UpdateExisting(void)
{
    // First set initial value
    hashmap_set(shell->env, "TEST_VAR", "old_value");
    
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("export");
    node->args[1] = ft_strdup("TEST_VAR=new_value");
    node->args[2] = NULL;

    int result = builtin_export(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    char *value = hashmap_get(shell->env, "TEST_VAR");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("new_value", value);
}

// UNSET Tests
void test_Builtin_Unset_ValidVar(void)
{
    // First set a variable
    hashmap_set(shell->env, "TEST_VAR", "value");

    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = ft_strdup("TEST_VAR");
    node->args[2] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_NULL(hashmap_get(shell->env, "TEST_VAR"));
}

void test_Builtin_Unset_NoArgs(void)
{
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);  // Should succeed with no arguments
}

void test_Builtin_Unset_NonexistentVar(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = ft_strdup("NONEXISTENT_VAR");
    node->args[2] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);  // Should succeed even if var doesn't exist
}

void test_Builtin_Unset_MultipleVars(void)
{
    // Set multiple variables
    hashmap_set(shell->env, "VAR1", "value1");
    hashmap_set(shell->env, "VAR2", "value2");
    hashmap_set(shell->env, "VAR3", "value3");

    node->args = malloc(5 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = ft_strdup("VAR1");
    node->args[2] = ft_strdup("VAR2");
    node->args[3] = ft_strdup("VAR3");
    node->args[4] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_NULL(hashmap_get(shell->env, "VAR1"));
    TEST_ASSERT_NULL(hashmap_get(shell->env, "VAR2"));
    TEST_ASSERT_NULL(hashmap_get(shell->env, "VAR3"));
}

void test_Builtin_Unset_InvalidIdentifier(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = ft_strdup("2INVALID");
    node->args[2] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should fail with invalid identifier
}

void test_Builtin_Unset_ReadOnlyVar(void)
{
    // Set up a read-only variable (if your shell supports it)
    hashmap_set(shell->env, "SHLVL", "1");  // SHLVL is typically read-only

    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("unset");
    node->args[1] = ft_strdup("SHLVL");
    node->args[2] = NULL;

    int result = builtin_unset(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    // Verify SHLVL is still set (if your shell protects read-only vars)
    TEST_ASSERT_NOT_NULL(hashmap_get(shell->env, "SHLVL"));
}

// ENV Tests
void test_Builtin_Env_NoArgs(void)
{
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("env");
    node->args[1] = NULL;

    int result = builtin_env(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Env_WithArgs(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("env");
    node->args[1] = ft_strdup("something");
    node->args[2] = NULL;

    int result = builtin_env(shell, node);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should fail with arguments
}

void test_Builtin_Env_AfterExport(void)
{
    // First export a new variable
    hashmap_set(shell->env, "TEST_VAR", "test_value");
    
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("env");
    node->args[1] = NULL;

    int result = builtin_env(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    // Verify the variable exists
    char *value = hashmap_get(shell->env, "TEST_VAR");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("test_value", value);
}

void test_Builtin_Env_AfterUnset(void)
{
    // First set and then unset a variable
    hashmap_set(shell->env, "TEST_VAR", "test_value");
    hashmap_remove(shell->env, "TEST_VAR");
    
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("env");
    node->args[1] = NULL;

    int result = builtin_env(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
    
    // Verify the variable doesn't exist
    char *value = hashmap_get(shell->env, "TEST_VAR");
    TEST_ASSERT_NULL(value);
}

// EXIT Tests
void test_Builtin_Exit_NoArgs(void)
{
    node->args = malloc(2 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_Builtin_Exit_WithValidCode(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = ft_strdup("42");
    node->args[2] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(42, result);
}

void test_Builtin_Exit_WithInvalidCode(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = ft_strdup("abc");
    node->args[2] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(255, result);  // Should exit with 255 for non-numeric arg
}

void test_Builtin_Exit_WithTooManyArgs(void)
{
    node->args = malloc(4 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = ft_strdup("42");
    node->args[2] = ft_strdup("extra");
    node->args[3] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(1, result);  // Should return 1 for too many arguments
}

void test_Builtin_Exit_WithNegativeNumber(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = ft_strdup("-42");
    node->args[2] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(214, result);  // -42 & 255 = 214 (8-bit conversion)
}

void test_Builtin_Exit_WithLargeNumber(void)
{
    node->args = malloc(3 * sizeof(char *));
    TEST_ASSERT_NOT_NULL(node->args);
    node->args[0] = ft_strdup("exit");
    node->args[1] = ft_strdup("300");
    node->args[2] = NULL;

    int result = builtin_exit(shell, node);
    TEST_ASSERT_EQUAL_INT(44, result);  // 300 % 256 = 44
}

int main(void)
{
    UNITY_BEGIN();
    
    // PWD Tests
    RUN_TEST(test_Builtin_PWD_ShouldReturnCurrentDirectory);
    
    // ECHO Tests
    RUN_TEST(test_Builtin_Echo_Basic);
    RUN_TEST(test_Builtin_Echo_NoArgs);
    RUN_TEST(test_Builtin_Echo_WithNewlineOption);
    RUN_TEST(test_Builtin_Echo_MultipleArgs);
    RUN_TEST(test_Builtin_Echo_WithMultipleNewlineOptions);
    RUN_TEST(test_Builtin_Echo_WithInvalidOption);
    
    // CD Tests
    RUN_TEST(test_Builtin_CD_ToHome);
    RUN_TEST(test_Builtin_CD_WithPath);
    RUN_TEST(test_Builtin_CD_NonexistentPath);
    RUN_TEST(test_Builtin_CD_ToOldPWD);
    
    // EXPORT Tests
    RUN_TEST(test_Builtin_Export_NoArgs);
    RUN_TEST(test_Builtin_Export_WithValidArg);
    RUN_TEST(test_Builtin_Export_WithMultipleArgs);
    RUN_TEST(test_Builtin_Export_WithInvalidIdentifier);
    RUN_TEST(test_Builtin_Export_WithoutValue);
    RUN_TEST(test_Builtin_Export_UpdateExisting);
    
    // UNSET Tests
    RUN_TEST(test_Builtin_Unset_ValidVar);
    RUN_TEST(test_Builtin_Unset_NoArgs);
    RUN_TEST(test_Builtin_Unset_NonexistentVar);
    RUN_TEST(test_Builtin_Unset_MultipleVars);
    RUN_TEST(test_Builtin_Unset_InvalidIdentifier);
    RUN_TEST(test_Builtin_Unset_ReadOnlyVar);
    
    // ENV Tests
    RUN_TEST(test_Builtin_Env_NoArgs);
    RUN_TEST(test_Builtin_Env_WithArgs);
    RUN_TEST(test_Builtin_Env_AfterExport);
    RUN_TEST(test_Builtin_Env_AfterUnset);
    
    // EXIT Tests
    RUN_TEST(test_Builtin_Exit_NoArgs);
    RUN_TEST(test_Builtin_Exit_WithValidCode);
    RUN_TEST(test_Builtin_Exit_WithInvalidCode);
    RUN_TEST(test_Builtin_Exit_WithTooManyArgs);
    RUN_TEST(test_Builtin_Exit_WithNegativeNumber);
    RUN_TEST(test_Builtin_Exit_WithLargeNumber);
    
    return UNITY_END();
} 