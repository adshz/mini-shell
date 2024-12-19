/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hashtable.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:55:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:55:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"
#include "hashtable.h"
#include "libft.h"

static size_t hash_function(const char *key, size_t table_size)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;
    unsigned int h = seed ^ ft_strlen(key);
    const unsigned char *data = (const unsigned char *)key;

    while (*key)
    {
        unsigned int k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        key++;
    }

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h % table_size;
}

t_hashmap *hashmap_create_table(size_t size)
{
    t_hashmap *table;
    size_t i;

    table = (t_hashmap *)malloc(sizeof(t_hashmap));
    if (!table)
        return NULL;
    table->items = (t_hash_item **)malloc(sizeof(t_hash_item *) * size);
    if (!table->items)
    {
        free(table);
        return NULL;
    }
    table->size = size;
    i = 0;
    while (i < size)
        table->items[i++] = NULL;
    return table;
}

int hashmap_insert(t_hashmap *table, char *key, char *value, int free_old)
{
    size_t index;
    t_hash_item *item;
    t_hash_item *new_item;
    char *new_value;
    char *new_key;

    if (!table || !key || !value)
        return -1;
    index = hash_function(key, table->size);
    item = table->items[index];
    // Check for existing key and update
    while (item)
    {
        if (ft_strcmp(item->key, key) == 0)
        {
            new_value = ft_strdup(value);
            if (!new_value)
                return -1;
            if (free_old)
                free(item->value);
            item->value = new_value;
            return 0;
        }
        item = item->next;
    }
    // Create new item
    new_item = (t_hash_item *)malloc(sizeof(t_hash_item));
    if (!new_item)
        return -1;
    new_key = ft_strdup(key);
    new_value = ft_strdup(value);
    if (!new_key || !new_value)
    {
        free(new_key);
        free(new_value);
        free(new_item);
        return -1;
    }
    new_item->key = new_key;
    new_item->value = new_value;
    new_item->next = table->items[index];
    table->items[index] = new_item;
    return 0;
}

char *hashmap_search(t_hashmap *table, char *key)
{
    size_t index;
    t_hash_item *item;

    if (!table || !key)
        return NULL;

    index = hash_function(key, table->size);
    item = table->items[index];
    while (item)
    {
        if (ft_strcmp(item->key, key) == 0)
            return item->value;
        item = item->next;
    }
    return NULL;
}

void hashmap_destroy(t_hashmap *table)
{
    size_t i;
    t_hash_item *item;
    t_hash_item *next;

    if (!table)
        return;
    for (i = 0; i < table->size; i++)
    {
        item = table->items[i];
        while (item)
        {
            next = item->next;
            free(item->key);
            free(item->value);
            free(item);
            item = next;
        }
    }
    free(table->items);
    free(table);
}

t_hashmap *env_to_hash(char **envp)
{
    t_hashmap *env;
    char *key;
    char *value;
    char *equals_pos;
    int i;
    size_t key_len;

    if (!envp)
        return NULL;

    env = hashmap_create_table(100);
    if (!env)
        return NULL;
    i = 0;
    while (envp[i])
    {
        equals_pos = ft_strchr(envp[i], '=');
        if (equals_pos)
        {
            key_len = equals_pos - envp[i];
            key = ft_substr(envp[i], 0, key_len);
            value = ft_strdup(equals_pos + 1);   
            if (!key || !value || hashmap_insert(env, key, value, 1) != 0)
            {
                free(key);
                free(value);
                hashmap_destroy(env);
                return NULL;
            }
            free(key);
            free(value);
        }
        i++;
    }
    return env;
}

t_hashmap *hashmap_create(void)
{
    t_hashmap *table;
    
    table = malloc(sizeof(t_hashmap));
    if (!table)
        return (NULL);
    table->size = HASH_SIZE;
    table->count = 0;
    table->items = calloc(table->size, sizeof(t_hash_item *));
    if (!table->items)
    {
        free(table);
        return (NULL);
    }
    return (table);
}

char *hashmap_get(t_hashmap *table, const char *key)
{
    size_t index;
    t_hash_item *item;

    if (!table || !key)
        return NULL;
    index = hash_function(key, table->size);
    item = table->items[index];
    while (item)
    {
        if (ft_strcmp(item->key, (char *)key) == 0)
            return item->value;
        item = item->next;
    }
    return NULL;
}

void hashmap_set(t_hashmap *table, const char *key, const char *value)
{
    size_t index;
    t_hash_item *item;
    t_hash_item *new_item;
    char *temp_key;

    if (!table || !key)
        return;
    
    temp_key = (char *)key;
    index = hash_function(key, table->size);
    item = table->items[index];
    while (item)
    {
        if (ft_strcmp(item->key, temp_key) == 0)
        {
            free(item->value);
            item->value = ft_strdup(value);
            return;
        }
        item = item->next;
    }
    new_item = (t_hash_item *)malloc(sizeof(t_hash_item));
    if (!new_item)
        return;

    new_item->key = ft_strdup(key);
    new_item->value = ft_strdup(value);
    if (!new_item->key || !new_item->value)
    {
        free(new_item->key);
        free(new_item->value);
        free(new_item);
        return;
    }

    new_item->next = table->items[index];
    table->items[index] = new_item;
}

void hashmap_remove(t_hashmap *table, const char *key)
{
    size_t index;
    t_hash_item *current;
    t_hash_item *prev;

    if (!table || !key)
        return;
    index = hash_function(key, table->size);
    current = table->items[index];
    prev = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, (char *)key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                table->items[index] = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

size_t hashmap_size(t_hashmap *table)
{
    size_t size = 0;
    size_t i;

    if (!table)
        return 0;
    for (i = 0; i < table->size; i++)
    {
        t_hash_item *item = table->items[i];
        while (item)
        {
            size++;
            item = item->next;
        }
    }
    return size;
}

void hashmap_iterate(t_hashmap *table, void (*f)(const char *, const char *))
{
    size_t i;
    t_hash_item *item;

    if (!table || !f)
        return;
    
    for (i = 0; i < table->size; i++)
    {
        item = table->items[i];
        while (item)
        {
            f(item->key, item->value);
            item = item->next;
        }
    }
}