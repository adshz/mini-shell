#include "hashtable.h"

static int process_env_entry(char *env_str, t_hashmap *env)
{
    char *equals_pos;
    char *key;
    char *value;
    size_t key_len;

    equals_pos = ft_strchr(env_str, '=');
    if (!equals_pos)
        return (0);
    key_len = equals_pos - env_str;
    key = ft_substr(env_str, 0, key_len);
    value = ft_strdup(equals_pos + 1);
    if (!key || !value || hashmap_insert(env, key, value, 1) != 0)
    {
        free(key);
        free(value);
        return (-1);
    }
    free(key);
    free(value);
    return (0);
}

t_hashmap *env_to_hash(char **envp)
{
    t_hashmap *env;
    int i;

    if (!envp || !(env = hashmap_create_table(100)))
        return (NULL);
    i = 0;
    while (envp[i])
    {
        if (process_env_entry(envp[i], env) != 0)
        {
            hashmap_destroy(env);
            return (NULL);
        }
        i++;
    }
    return (env);
}