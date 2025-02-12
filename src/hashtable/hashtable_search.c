/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_search.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:36:13 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 15:38:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"

/**
 * @brief Searches for a value in the hash table by key
 *
 * Performs lookup in the hash table:
 * - Computes hash index for the key
 * - Traverses the chain at that index
 * - Returns the value if key is found
 *
 * @param table Hash table to search in
 * @param key   Key to search for
 * @return      Value associated with the key, or NULL if:
 *              - Key not found
 *              - Table is NULL
 *              - Key is NULL
 *
 * @note Does not modify the hash table
 * @note Returns direct pointer to stored value, do not free
 */
char	*hashmap_search(t_hashmap *table, char *key)
{
	size_t		index;
	t_hash_item	*item;

	if (!table || !key)
		return (NULL);
	index = hash_function(key, table->size);
	item = table->items[index];
	while (item)
	{
		if (ft_strcmp(item->key, key) == 0)
			return (item->value);
		item = item->next;
	}
	return (NULL);
}
