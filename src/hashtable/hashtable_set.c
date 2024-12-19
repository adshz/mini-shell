/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_set.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:29 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:44:29 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include "libft.h"

static int update_item_value(t_hash_item *item, const char *value)
{
    char *new_value;

    free(item->value);
    new_value = ft_strdup(value);
    if (!new_value)
        return (-1);
    item->value = new_value;
    return (0);
}

static t_hash_item *create_item(const char *key, const char *value)
{
    t_hash_item *new_item;

    new_item = (t_hash_item *)malloc(sizeof(t_hash_item));
    if (!new_item)
        return (NULL);
    new_item->key = ft_strdup(key);
    new_item->value = ft_strdup(value);
    if (!new_item->key || !new_item->value)
    {
        free(new_item->key);
        free(new_item->value);
        free(new_item);
        return (NULL);
    }
    return (new_item);
}

void hashmap_set(t_hashmap *table, const char *key, const char *value)
{
    size_t index;
    t_hash_item *item;
    t_hash_item *new_item;

    if (!table || !key)
        return;
    index = hash_function(key, table->size);
    item = table->items[index];
    while (item && ft_strcmp(item->key, key) != 0)
        item = item->next;
    if (item && update_item_value(item, value) == 0)
        return;
    new_item = create_item(key, value);
    if (!new_item)
        return;
    new_item->next = table->items[index];
    table->items[index] = new_item;
}
