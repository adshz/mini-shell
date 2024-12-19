/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:43:54 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:45:06 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include "libft.h"
t_hashmap *hashmap_create_table(size_t size)
{
    t_hashmap *table;

    table = malloc(sizeof(t_hashmap));
    if (!table)
        return (NULL);
    table->size = size;
    table->count = 0;
    table->items = ft_calloc(size, sizeof(t_hash_item *));
    if (!table->items)
    {
        free(table);
        return (NULL);
    }
    return (table);
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
