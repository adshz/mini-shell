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

t_hash_item	*hashmap_get_item(t_hashmap *table, const char *key)
{
	size_t		index;
	t_hash_item	*current;

	if (!table || !key)
		return (NULL);
	index = hash_function(key, table->size);
	current = table->items[index];
	while (current)
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}
