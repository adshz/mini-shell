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

static int	validate_hashmap(t_hashmap *table)
{
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
	return (1);
}

static int	validate_current_item(t_hash_count *hc, t_hashmap *table)
{
	if (hc->items_in_bucket >= hc->max_items_per_bucket)
	{
		ft_putendl_fd("Bucket corrupted: too many items", 2);
		return (0);
	}
	if (!hc->current->key || !hc->current->value)
	{
		ft_putendl_fd("Encountered corrupted item", 2);
		return (0);
	}
	if (hc->current->next && (hc->current->next == hc->current || \
		(void *)hc->current->next < (void *)table || \
		(void *)hc->current->next > (void *)table + \
		(HASH_SIZE * sizeof(t_hash_item *))))
	{
		ft_putendl_fd("Invalid next pointer detected", 2);
		return (0);
	}
	return (1);
}

static void	process_item(t_hash_count *hc)
{
	hc->count++;
	hc->items_in_bucket++;
	hc->current = hc->current->next;
}

static void	count_bucket_items(t_hash_count *hc, t_hashmap *table)
{
	hc->items_in_bucket = 0;
	hc->current = table->items[hc->i];
	while (hc->current != NULL)
	{
		if (!validate_current_item(hc, table))
			break ;
		process_item(hc);
	}
}

size_t	hashmap_size(t_hashmap *table)
{
	t_hash_count	hc;

	if (!validate_hashmap(table))
		return (0);
	hc.count = 0;
	hc.i = 0;
	hc.max_items_per_bucket = 1000;
	while (hc.i < table->size)
	{
		count_bucket_items(&hc, table);
		hc.i++;
	}
	return (hc.count);
}
