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
		return (0);
	if (table->size == 0 || table->size > HASH_SIZE)
		return (0);
	if (!table->items)
		return (0);
	return (1);
}

static int	validate_current_item(t_hash_count *hc, t_hashmap *table)
{
	if (!hc->current || !table)
		return (0);
	if (hc->items_in_bucket >= hc->max_items_per_bucket)
		return (0);
	if (!hc->current->key || !hc->current->value)
		return (0);
	if (hc->current->next && hc->current->next == hc->current)
		return (0);
	return (1);
}

static void	process_item(t_hash_count *hc)
{
	if (hc && hc->current)
	{
		hc->count++;
		hc->items_in_bucket++;
		hc->current = hc->current->next;
	}
}

static void	count_bucket_items(t_hash_count *hc, t_hashmap *table)
{
	if (!hc || !table)
		return ;
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
	ft_memset(&hc, 0, sizeof(t_hash_count));
	hc.max_items_per_bucket = 1000;
	while (hc.i < table->size)
	{
		count_bucket_items(&hc, table);
		hc.i++;
	}
	return (hc.count);
}
