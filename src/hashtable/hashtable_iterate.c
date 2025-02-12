/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_iterate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:11 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:44:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

void	hashmap_iterate(t_hashmap *table, void (*f)(const char *, const char *))
{
	size_t		i;
	t_hash_item	*item;

	if (!table || !f)
		return ;
	i = 0;
	while (i < table->size)
	{
		item = table->items[i];
		while (item)
		{
			f(item->key, item->value);
			item = item->next;
		}
		i++;
	}
}
