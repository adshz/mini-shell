#include "unity_config.h"
#include "unity.h"
#include "shell.h"
#include "lexer.h"
#include "parser.h"
#include <stdlib.h>

// Global variables for testing
static t_token *test_tokens;
static t_ast_node *test_ast;

void setUp(void)
{
    // Initialize test variables before each test
    test_tokens = NULL;
    test_ast = NULL;
}

void tearDown(void)
{
    // Clean up after each test
    if (test_tokens)
    {
        free_tokens(test_tokens);
        test_tokens = NULL;
    }
    if (test_ast)
    {
        free_ast(test_ast);
        test_ast = NULL;
    }
}

void test_Parser_ShouldHandleSimpleCommand(void)
{
    t_token *tokens = tokenise("ls -l");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->type);
    TEST_ASSERT_EQUAL_STRING("ls", ast->value);
    TEST_ASSERT_NOT_NULL(ast->args);
    TEST_ASSERT_EQUAL_STRING("ls", ast->args[0]);
    TEST_ASSERT_EQUAL_STRING("-l", ast->args[1]);
    TEST_ASSERT_NULL(ast->args[2]);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Parser_ShouldHandlePipe(void)
{
    t_token *tokens = tokenise("ls -l | grep test");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(AST_PIPE, ast->type);
    
    // Check left side (ls -l)
    TEST_ASSERT_NOT_NULL(ast->left);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->left->type);
    TEST_ASSERT_EQUAL_STRING("ls", ast->left->value);
    TEST_ASSERT_NOT_NULL(ast->left->args);
    TEST_ASSERT_EQUAL_STRING("ls", ast->left->args[0]);
    TEST_ASSERT_EQUAL_STRING("-l", ast->left->args[1]);
    TEST_ASSERT_NULL(ast->left->args[2]);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Parser_ShouldHandleRedirection(void)
{
    t_token *tokens = tokenise("echo hello > output.txt");
    t_ast_node *ast = parse(tokens);
    
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(AST_REDIR_OUT, ast->type);
    
    // Check left side (echo hello)
    TEST_ASSERT_NOT_NULL(ast->left);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->left->type);
    TEST_ASSERT_EQUAL_STRING("echo", ast->left->value);
    TEST_ASSERT_NOT_NULL(ast->left->args);
    TEST_ASSERT_EQUAL_STRING("echo", ast->left->args[0]);
    TEST_ASSERT_EQUAL_STRING("hello", ast->left->args[1]);
    TEST_ASSERT_NULL(ast->left->args[2]);
    
    // Check right side (output.txt)
    TEST_ASSERT_NOT_NULL(ast->right);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->right->type);
    TEST_ASSERT_EQUAL_STRING("output.txt", ast->right->value);
    
    free_ast(ast);
    free_tokens(tokens);
}

void test_Parser_ShouldHandleComplexCommand(void)
{
    // Create tokens for: "cat file.txt | grep pattern > output.txt"
    t_token *tokens = create_token(TOKEN_WORD, "cat");
    TEST_ASSERT_NOT_NULL(tokens);
    
    tokens->next = create_token(TOKEN_WORD, "file.txt");
    TEST_ASSERT_NOT_NULL(tokens->next);
    
    tokens->next->next = create_token(TOKEN_PIPE, "|");
    TEST_ASSERT_NOT_NULL(tokens->next->next);
    
    tokens->next->next->next = create_token(TOKEN_WORD, "grep");
    TEST_ASSERT_NOT_NULL(tokens->next->next->next);
    
    tokens->next->next->next->next = create_token(TOKEN_WORD, "pattern");
    TEST_ASSERT_NOT_NULL(tokens->next->next->next->next);
    
    tokens->next->next->next->next->next = create_token(TOKEN_REDIRECT_OUT, ">");
    TEST_ASSERT_NOT_NULL(tokens->next->next->next->next->next);
    
    tokens->next->next->next->next->next->next = create_token(TOKEN_WORD, "output.txt");
    TEST_ASSERT_NOT_NULL(tokens->next->next->next->next->next->next);

    t_ast_node *ast = parse(tokens);
    
    // Verify the structure
    TEST_ASSERT_NOT_NULL(ast);
    TEST_ASSERT_EQUAL(AST_REDIR_OUT, ast->type);
    
    // Check pipe node
    TEST_ASSERT_NOT_NULL(ast->left);
    TEST_ASSERT_EQUAL(AST_PIPE, ast->left->type);
    
    // Check left side of pipe (cat command)
    TEST_ASSERT_NOT_NULL(ast->left->left);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->left->left->type);
    TEST_ASSERT_EQUAL_STRING("cat", ast->left->left->value);
    TEST_ASSERT_NOT_NULL(ast->left->left->args);
    TEST_ASSERT_EQUAL_STRING("cat", ast->left->left->args[0]);
    TEST_ASSERT_EQUAL_STRING("file.txt", ast->left->left->args[1]);
    
    // Check right side of pipe (grep command)
    TEST_ASSERT_NOT_NULL(ast->left->right);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->left->right->type);
    TEST_ASSERT_EQUAL_STRING("grep", ast->left->right->value);
    TEST_ASSERT_NOT_NULL(ast->left->right->args);
    TEST_ASSERT_EQUAL_STRING("grep", ast->left->right->args[0]);
    TEST_ASSERT_EQUAL_STRING("pattern", ast->left->right->args[1]);
    
    // Check redirection
    TEST_ASSERT_NOT_NULL(ast->right);
    TEST_ASSERT_EQUAL(AST_COMMAND, ast->right->type);
    TEST_ASSERT_EQUAL_STRING("output.txt", ast->right->value);
    
    free_ast(ast);
    free_tokens(tokens);
} 


int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Parser_ShouldHandleSimpleCommand);
    RUN_TEST(test_Parser_ShouldHandlePipe);
    RUN_TEST(test_Parser_ShouldHandleRedirection);
    RUN_TEST(test_Parser_ShouldHandleComplexCommand);
    return UNITY_END();
}