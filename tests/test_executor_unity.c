#include "unity_config.h"
#include "unity.h"
#include "shell.h"
#include "executor.h"
#include "parser.h"
#include "lexer.h"
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

// Global variables for testing
static t_shell *shell;
static char *test_env[] = {"PATH=/bin:/usr/bin", NULL};

void setUp(void)
{
    // Initialize shell for each test
    shell = malloc(sizeof(t_shell));
    TEST_ASSERT_NOT_NULL(shell);
    
    // Create dummy argv for init_shell
    char *argv[] = {"minishell", NULL};
    
    // Call init_shell with all required arguments
    init_shell(shell, argv, test_env);
}

void tearDown(void)
{
    // Cleanup after each test
    if (shell)
    {
        cleanup_shell(shell);
        free(shell);
        shell = NULL;
    }
}

void test_Executor_ShouldHandleSimpleCommand(void)
{
    // Test simple command: "ls -l"
    t_token *tokens = tokenise("ls -l");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(0, status);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Executor_ShouldHandlePipe(void)
{
    // Test pipe: "ls -l | grep test"
    t_token *tokens = tokenise("ls -l | grep test");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(0, status);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Executor_ShouldHandleRedirectionOut(void)
{
    // Test redirection: "echo hello > test.txt"
    char *test_file = "test.txt";
    t_token *tokens = tokenise("echo hello > test.txt");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(0, status);
    
    // Verify file contents
    int fd = open(test_file, O_RDONLY);
    TEST_ASSERT_NOT_EQUAL(-1, fd);
    
    char buffer[256] = {0};
    ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
    TEST_ASSERT_GREATER_THAN(0, bytes_read);
    
    TEST_ASSERT_EQUAL_STRING("hello\n", buffer);
    
    close(fd);
    unlink(test_file);  // Clean up test file
    free_ast(ast);
    free_tokens(tokens);
}

void test_Executor_ShouldHandleRedirectionIn(void)
{
    // Create a test input file
    char *test_input = "test_input.txt";
    int fd = open(test_input, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    TEST_ASSERT_NOT_EQUAL(-1, fd);
    write(fd, "hello\n", 6);
    close(fd);
    
    // Test input redirection: "cat < test_input.txt"
    t_token *tokens = tokenise("cat < test_input.txt");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(0, status);
    
    unlink(test_input);  // Clean up test file
    free_ast(ast);
    free_tokens(tokens);
}

void test_Executor_ShouldHandleCommandNotFound(void)
{
    // Test non-existent command
    t_token *tokens = tokenise("nonexistentcommand");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(EXIT_CMD_NOT_FOUND, status);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Executor_ShouldHandleComplexPipeline(void)
{
    // Test complex pipeline: "ls -l | grep test | wc -l > count.txt"
    t_token *tokens = tokenise("ls -l | grep test | wc -l > count.txt");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    
    int status = execute_ast(shell, ast);
    TEST_ASSERT_EQUAL(0, status);
    
    // Verify the output file exists
    TEST_ASSERT_EQUAL(0, access("count.txt", F_OK));
    
    unlink("count.txt");  // Clean up test file
    free_ast(ast);
    free_tokens(tokens);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Executor_ShouldHandleSimpleCommand);
    RUN_TEST(test_Executor_ShouldHandlePipe);
    RUN_TEST(test_Executor_ShouldHandleRedirectionOut);
    RUN_TEST(test_Executor_ShouldHandleRedirectionIn);
    RUN_TEST(test_Executor_ShouldHandleCommandNotFound);
    RUN_TEST(test_Executor_ShouldHandleComplexPipeline);
    return UNITY_END();
} 