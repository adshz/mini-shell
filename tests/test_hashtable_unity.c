#include "../external/Unity/src/unity.h"
#include "../inc/hashtable.h"
#include "../inc/shell.h"
#include "../libft/inc/libft.h"
#include <stdlib.h>

void setUp(void)
{
    // This is run before each test
}

void tearDown(void)
{
    // This is run after each test
}

void test_basic_environment_conversion(void)
{
    char **test_env;
    t_hashmap *env;
    char *path;
    char *home;
    char *user;

    // Setup
    test_env = (char **)malloc(sizeof(char *) * 4);
    TEST_ASSERT_NOT_NULL(test_env);
    
    test_env[0] = ft_strdup("USER=test");
    test_env[1] = ft_strdup("PATH=/usr/bin:bin");
    test_env[2] = ft_strdup("HOME=/home/test");
    test_env[3] = NULL;

    // Test hashtable creation
    env = env_to_hash(test_env);
    TEST_ASSERT_NOT_NULL(env);

    // Test environment variables
    path = hashmap_search(env, "PATH");
    TEST_ASSERT_NOT_NULL(path);
    TEST_ASSERT_EQUAL_STRING("/usr/bin:bin", path);

    home = hashmap_search(env, "HOME");
    TEST_ASSERT_NOT_NULL(home);
    TEST_ASSERT_EQUAL_STRING("/home/test", home);

    user = hashmap_search(env, "USER");
    TEST_ASSERT_NOT_NULL(user);
    TEST_ASSERT_EQUAL_STRING("test", user);

    // Cleanup
    for (int i = 0; test_env[i]; i++)
        free(test_env[i]);
    free(test_env);
    hashmap_destroy(env);
}

void test_empty_environment(void)
{
    char **test_env;
    t_hashmap *env;

    // Setup
    test_env = (char **)malloc(sizeof(char *) * 1);
    TEST_ASSERT_NOT_NULL(test_env);
    test_env[0] = NULL;

    // Test hashtable creation
    env = env_to_hash(test_env);
    TEST_ASSERT_NOT_NULL(env);

    // Test empty environment
    TEST_ASSERT_NULL(hashmap_search(env, "USER"));
    TEST_ASSERT_NULL(hashmap_search(env, "PATH"));
    TEST_ASSERT_NULL(hashmap_search(env, "HOME"));

    // Cleanup
    free(test_env);
    hashmap_destroy(env);
}

void test_null_environment(void)
{
    t_hashmap *env;

    env = env_to_hash(NULL);
    TEST_ASSERT_NULL(env);
}

void test_hashtable_updates(void)
{
    t_hashmap *env;
    char *value;

    // Create hashtable
    env = hashmap_create_table(10);
    TEST_ASSERT_NOT_NULL(env);

    // Test initial insert
    TEST_ASSERT_EQUAL_INT(0, hashmap_insert(env, "TEST", "value1", 0));
    value = hashmap_search(env, "TEST");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("value1", value);

    // Test update
    TEST_ASSERT_EQUAL_INT(0, hashmap_insert(env, "TEST", "value2", 0));
    value = hashmap_search(env, "TEST");
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING("value2", value);

    // Cleanup
    hashmap_destroy(env);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_basic_environment_conversion);
    RUN_TEST(test_empty_environment);
    RUN_TEST(test_null_environment);
    RUN_TEST(test_hashtable_updates);
    return UNITY_END();
} 