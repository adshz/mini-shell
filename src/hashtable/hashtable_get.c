/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 hashtable_get.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/01/29 14:48:33 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/01/29 14:48:33 by szhong			  ###	########.fr		  */
/*																			  */
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
