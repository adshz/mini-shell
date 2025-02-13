/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_get.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:24:21 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:24:55 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

char	*hashmap_get(t_hashmap *table, const char *key)
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
