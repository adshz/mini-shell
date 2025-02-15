/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_size.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 18:35:47 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

size_t	hashmap_size(t_hashmap *table)
{
	size_t		count;
	size_t		i;
	t_hash_item	*current;
	size_t		max_items_per_bucket;

	if (!table)
	{
		ft_putendl_fd("Table pointer is NULL in hashmap_size", 2);
		return (0);
	}
	if (table->size == 0 || table->size > HASH_SIZE)
	{
		ft_putendl_fd("Invalid table size", 2);
		return (0);
	}
	if (!table->items)
	{
		ft_putendl_fd("Items array is NULL in hashmap_size", 2);
		return (0);
	}
	count = 0;
	i = 0;
	max_items_per_bucket = 1000;
	while (i < table->size)
	{
		size_t	items_in_bucket = 0;
		current = table->items[i];
		while (current != NULL)
		{
			if (items_in_bucket >= max_items_per_bucket)
			{
				ft_putendl_fd("Bucket corrupted: too many items", 2);
				return (count);
			}
			if (!current->key || !current->value)
			{
				ft_putendl_fd("Encountered corrupted item", 2);
				break ;
			}
			count++;
			items_in_bucket++;
			if (current->next && (current->next == current ||
				(void*)current->next < (void*)table ||
				(void*)current->next > (void*)table + (HASH_SIZE * sizeof(t_hash_item*))))
			{
				ft_putendl_fd("Invalid next pointer detected", 2);
				break ;
			}
			current = current->next;
		}
		i++;
	}
	return (count);
}
