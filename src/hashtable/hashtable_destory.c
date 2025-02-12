/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_destory.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 11:37:05 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 12:00:50 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable/hashtable.h"

/**
 * @brief Free all memory in a linked list of hash items
 *
 * This helper function cleans up a chain of collided items:
 * @code
 *   Before:
 *   item1 -> item2 -> item3 -> NULL
 *
 *   Step 1: Save next pointer
 *   next = item2
 *
 *   Step 2: Fre current item
 *   free(item1->key)
 *   free(item1->value)
 *   free(item1)
 *
 *   Step 3: Move to next pointer
 *   item = next
 * @endcode
 *
 * @param item Start of the item chain to free
 * @see hashmao_destory() Main cleanup function that use free_item_chain
 */
static void	free_item_chain(t_hash_item *item)
{
	t_hash_item	*next;

	while (item)
	{
		next = item->next;
		free(item->key);
		free(item->value);
		free(item);
		item = next;
	}
}

/**
 * @brief Cleans up the entire hash table and all its content
 *
 * Memory cleanup process:\n
 * @code
 *  // Example table with size 4
 *  table->items = [slot0, slot1, slot2, slot3]
 *                   |       |      |      |
 *                 item1->item2->NULL->item4->NULL
 *                  |       |            |
 *                 NULL   NULL         NULL
 * While loop steps:\n
 *   i = 0: free_item_chain(slot0) // free item1
 *   i = 1: free_item_chain(slot1) // free item2
 *   i = 2: free_item_chain(NULL)  // nothing to free
 *   i = 3: free_item_chain(slot3) // free item3
 * @endcode
 * @param table The hash table to destroy (can be NULL)
 *
 * @note Safe to pass NULL table
 * @see free_item_chain() Frees individual chains regarding the index
 */
void	hashmap_destroy(t_hashmap *table)
{
	size_t	i;

	if (!table)
		return ;
	i = 0;
	while (i < table->size)
	{
		free_item_chain(table->items[i]);
		i++;
	}
	free(table->items);
	free(table);
}
