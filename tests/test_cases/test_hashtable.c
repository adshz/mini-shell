#include "ft_test.h"
#include "shell.h"
#include "hashtable.h"
#include <stdio.h>

/* Test basic hashtable creation*/
static void test_env_to_hash_basic(t_test_result *result)
{
    char    **test_env;
    t_hashmap *env;
    char    *path;
    char    *home;
    char    *user;

    printf("\nRunning basic hashtable test:\n");
    //setup 
    test_env = (char **)malloc(sizeof(char *) * 4);
    if (!test_env)
    {
        printf("Failed to allocate test_env\n");
        assert_true(result, 0, "Memory allocation failed");
        return;
    }
    test_env[0] = ft_strdup("USER=test");
    test_env[1] = ft_strdup("PATH=/usr/bin:bin");
    test_env[2] = ft_strdup("HOME=/home/test");
    test_env[3] = NULL;

    printf("Created test environment\n");
    env = env_to_hash(test_env);
    assert_true(result, env != NULL, "Hashtable creation failed");
    if (!env)
    {
        for (int i = 0; test_env[i]; i++)
            free(test_env[i]);
        free(test_env);
        return;
    }
    
    printf("Searching for PATH\n");
    path = hashmap_search(env, "PATH");
    printf("PATH value found: '%s'\n", path ? path : "NULL");
    printf("Expected PATH: '/usr/bin:bin'\n");
    printf("Comparison result: %d\n", ft_strcmp(path, "/usr/bin:bin"));
    assert_str_eq(result, "/usr/bin:bin", path, "PATH value is incorrect");

    printf("Searching for HOME\n");
    home = hashmap_search(env, "HOME");
    printf("HOME value found: '%s'\n", home ? home : "NULL");
    printf("Expected HOME: '/home/test'\n");
    printf("Comparison result: %d\n", ft_strcmp(home, "/home/test"));
    assert_str_eq(result, "/home/test", home, "HOME value is incorrect");

    printf("Searching for USER\n");
    user = hashmap_search(env, "USER");
    printf("USER value found: '%s'\n", user ? user : "NULL");
    printf("Expected USER: 'test'\n");
    printf("Comparison result: %d\n", ft_strcmp(user, "test"));
    assert_str_eq(result, "test", user, "USER value is incorrect");

    //cleanup
    printf("Cleaning up\n");
    for (int i = 0; test_env[i]; i++)
        free(test_env[i]);
    free(test_env);
    hashmap_destroy(env);
}

/* Test env_to_hash with an empty env */
static void test_env_to_hash_empty(t_test_result *result)
{
    char **test_env;
    t_hashmap *env;

    printf("\nRunning empty env test:\n");
    //setup
    test_env = (char **)malloc(sizeof(char *) * 1);
    if (!test_env)
    {
        assert_true(result, 0, "Memory allocation failed");
        return;
    }
    test_env[0] = NULL;

    env = env_to_hash(test_env);
    assert_true(result, env != NULL, "Empty hashtable creation failed");
    if (!env)
    {
        free(test_env);
        return;
    }
    
    printf("Checking empty env values\n");
    char *user = hashmap_search(env, "USER");
    char *path = hashmap_search(env, "PATH");
    char *home = hashmap_search(env, "HOME");
    
    printf("USER value: '%s'\n", user ? user : "NULL");
    printf("PATH value: '%s'\n", path ? path : "NULL");
    printf("HOME value: '%s'\n", home ? home : "NULL");
    
    assert_true(result, user == NULL, "Empty env should not contain USER");
    assert_true(result, path == NULL, "Empty env should not contain PATH");
    assert_true(result, home == NULL, "Empty env should not contain HOME");
    
    free(test_env);
    hashmap_destroy(env);
}

/* Test env_to_hash with a NULL env */
static void test_env_to_hash_null(t_test_result *result)
{
    t_hashmap *env;

    printf("\nRunning NULL env test:\n");
    env = env_to_hash(NULL);
    printf("env_to_hash(NULL) returned: %p\n", (void*)env);
    assert_true(result, env == NULL, "env_to_hash should return NULL for NULL env");
    if (env)
        hashmap_destroy(env);
}

/* Test environment variable updates*/
static void test_hashmap_update(t_test_result *result)
{
    t_hashmap *env;
    char *value;

    printf("\nRunning update test:\n");
    env = hashmap_create_table(10);
    assert_true(result, env != NULL, "Hashtable creation failed");
    if (!env)
        return;

    printf("Inserting initial value\n");
    assert_true(result, hashmap_insert(env, "TEST", "value1", 0) == 0, "Initial insert failed");
    value = hashmap_search(env, "TEST");
    printf("Initial value: '%s'\n", value ? value : "NULL");
    printf("Expected value: 'value1'\n");
    printf("Comparison result: %d\n", ft_strcmp(value, "value1"));
    assert_str_eq(result, "value1", value, "Initial value is incorrect");

    printf("Updating value\n");
    assert_true(result, hashmap_insert(env, "TEST", "value2", 0) == 0, "Update failed");
    value = hashmap_search(env, "TEST");
    printf("Updated value: '%s'\n", value ? value : "NULL");
    printf("Expected value: 'value2'\n");
    printf("Comparison result: %d\n", ft_strcmp(value, "value2"));
    assert_str_eq(result, "value2", value, "Updated value is incorrect");

    hashmap_destroy(env);
}

/* Register all hashtable tests */
void register_hashtable_tests(t_test_case **suite)
{
    test_add(suite, test_create("Basic Environment Conversion", test_env_to_hash_basic));
    test_add(suite, test_create("Empty Environment", test_env_to_hash_empty));
    test_add(suite, test_create("When Environment is NULL", test_env_to_hash_null));
    test_add(suite, test_create("Hashtable Updates", test_hashmap_update));
} 