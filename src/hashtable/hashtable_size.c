/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:32 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:44:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"
#include <stdio.h>
size_t	hashmap_size(t_hashmap *table)
{
	size_t		count;
	size_t		i;
	t_hash_item	*current;
	size_t		max_items_per_bucket;

	if (!table)
	{
		fprintf(stderr, "DEBUG: Table pointer is NULL in hashmap_size\n");
		return (0);
	}
	fprintf(stderr, "DEBUG: Table pointer: %p\n", (void*)table);
	fprintf(stderr, "DEBUG: Table size value: %zu\n", table->size);
	fprintf(stderr, "DEBUG: Table items pointer: %p\n", (void*)table->items);

	// Add strict size validation
	if (table->size == 0 || table->size > HASH_SIZE)
	{
		fprintf(stderr, "DEBUG: Invalid table size: %zu\n", table->size);
		return (0);
	}

    	if (!table->items)
	{
		fprintf(stderr, "DEBUG: Items array is NULL in hashmap_size\n");
		return (0);
	}

	count = 0;
	i = 0;
	max_items_per_bucket = 1000; // Safety limit to prevent infinite loops
	fprintf(stderr, "DEBUG: Starting to iterate through hash table (size: %zu)\n", table->size);
	
	while (i < table->size)
	{
		size_t items_in_bucket = 0;
		if (i % 100 == 0)
			fprintf(stderr, "DEBUG: Processing bucket group %zu-%zu\n", i, i + 99);
		
		current = table->items[i];
		while (current != NULL)
		{
			// Check for corrupted linked list
			if (items_in_bucket >= max_items_per_bucket)
			{
				fprintf(stderr, "DEBUG: Possible corrupted linked list in bucket %zu (too many items)\n", i);
				return count;
			}

			// Validate current item's pointers
			if (!current->key || !current->value)
			{
				fprintf(stderr, "DEBUG: Found corrupted item in bucket %zu\n", i);
				break;
			}

			fprintf(stderr, "DEBUG: Found item in bucket %zu: key='%s', value='%s'\n", 
				i, current->key, current->value);
			
			count++;
			items_in_bucket++;
			
			// Validate next pointer before advancing
			if (current->next && (current->next == current || 
				(void*)current->next < (void*)table || 
				(void*)current->next > (void*)table + (HASH_SIZE * sizeof(t_hash_item*))))
			{
				fprintf(stderr, "DEBUG: Detected invalid next pointer in bucket %zu\n", i);
				break;
			}
			
			current = current->next;
		}
		i++;
	}
	fprintf(stderr, "DEBUG: Counted %zu items in hash table\n", count);
	return (count);
}
