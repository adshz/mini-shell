/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_remove.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 15:39:38 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 15:43:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"

/**
 * @brief Removes an item from the hash table by key
 *
 * Handles removal of items while maintaining the linked list structure:
 * - Computes hash index for the key
 * - Traverses the chain at that index
 * - Updates pointers to maintain chain integrity
 * - Frees the removed item
 *
 * @param table Hash table to remove from
 * @param key   Key of the item to remove
 *
 * @note Safe to call with NULL table or key
 * @note If key doesn't exist, function silently returns
 * @note Updates chain pointers to maintain linked list structure
 */
void	hashmap_remove(t_shell *shell, t_hashmap *table, const char *key)
{
	size_t		idx;
	t_hash_item	*current;
	t_hash_item	*prev;

	if (!table || !key)
		return ;
	idx = hash_function(key, table->size);
	current = table->items[idx];
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				table->items[idx] = current->next;
			hashmap_free_item(shell, &current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}
