/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_create.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:29:06 by szhong            #+#    #+#             */
/*   Updated: 2025/01/22 16:13:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"

/**
 * @brief Creates a hash table with specified size
 *
 * Allocats and initailises a new hash table structure with a custom size
 * The table's item array is initialised with ft_calloc from libft to ensure 
 * all pointers are set to NULL;
 *
 * @param param_name Parameter description
 * @return Return value description
 */
t_hashmap	*hashmap_create_table(size_t size)
{
	t_hashmap	*table;

	if (size < 1)
		return (NULL);
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

t_hashmap	*hashmap_create(void)
{
	return (hashmap_create_table(HASH_SIZE));
}
