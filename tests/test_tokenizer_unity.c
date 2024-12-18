#include "unity.h"
#include "shell.h"
#include "lexer.h"
#include <stdlib.h>

void setUp(void) {
    // Setup code if needed
}

void tearDown(void) {
    // Cleanup code if needed
}

void test_Tokenizer_ShouldHandleSimpleCommand(void)
{
    t_token *tokens = tokenise("ls -l");
    
    TEST_ASSERT_NOT_NULL(tokens);
    TEST_ASSERT_EQUAL_STRING("ls", tokens->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next);
    TEST_ASSERT_EQUAL_STRING("-l", tokens->next->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->next->type);
    
    TEST_ASSERT_NULL(tokens->next->next);
    
    free_tokens(tokens);
}

void test_Tokenizer_ShouldHandleRedirections(void)
{
    t_token *tokens = tokenise("echo hello > output.txt");
    
    TEST_ASSERT_NOT_NULL(tokens);
    TEST_ASSERT_EQUAL_STRING("echo", tokens->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next);
    TEST_ASSERT_EQUAL_STRING("hello", tokens->next->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->next->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next->next);
    TEST_ASSERT_EQUAL_STRING(">", tokens->next->next->value);
    TEST_ASSERT_EQUAL(TOKEN_REDIRECT_OUT, tokens->next->next->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next->next->next);
    TEST_ASSERT_EQUAL_STRING("output.txt", tokens->next->next->next->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->next->next->next->type);
    
    TEST_ASSERT_NULL(tokens->next->next->next->next);
    
    free_tokens(tokens);
}

void test_Tokenizer_ShouldHandlePipes(void)
{
    t_token *tokens = tokenise("ls | grep test");
    
    TEST_ASSERT_NOT_NULL(tokens);
    TEST_ASSERT_EQUAL_STRING("ls", tokens->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next);
    TEST_ASSERT_EQUAL_STRING("|", tokens->next->value);
    TEST_ASSERT_EQUAL(TOKEN_PIPE, tokens->next->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next->next);
    TEST_ASSERT_EQUAL_STRING("grep", tokens->next->next->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->next->next->type);
    
    TEST_ASSERT_NOT_NULL(tokens->next->next->next);
    TEST_ASSERT_EQUAL_STRING("test", tokens->next->next->next->value);
    TEST_ASSERT_EQUAL(TOKEN_WORD, tokens->next->next->next->type);
    
    TEST_ASSERT_NULL(tokens->next->next->next->next);
    
    free_tokens(tokens);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_Tokenizer_ShouldHandleSimpleCommand);
    RUN_TEST(test_Tokenizer_ShouldHandleRedirections);
    RUN_TEST(test_Tokenizer_ShouldHandlePipes);
    return UNITY_END();
} 