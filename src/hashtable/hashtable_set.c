/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 hashtable_set.c									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/18 16:05:16 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/18 16:05:16 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "hashtable.h"

static int	update_item_value(t_hash_item *item, const char *value)
{
	char	*new_value;

	if (!item || !value)
		return (-1);
	new_value = ft_strdup(value);
	if (!new_value)
		return (-1);
	free(item->value);
	item->value = new_value;
	return (0);
}

static t_hash_item	*create_item(const char *key, const char *value)
{
	t_hash_item	*new_item;

	if (!key || !value)
		return (NULL);
	new_item = (t_hash_item *)malloc(sizeof(t_hash_item));
	if (!new_item)
		return (NULL);
	new_item->key = ft_strdup(key);
	new_item->value = ft_strdup(value);
	new_item->next = NULL;
	if (!new_item->key || !new_item->value)
	{
		free(new_item->key);
		free(new_item->value);
		free(new_item);
		return (NULL);
	}
	return (new_item);
}

void	hashmap_set(t_hashmap *table, const char *key, const char *value)
{
	size_t		index;
	t_hash_item	*item;
	t_hash_item	*new_item;

	if (!table || !key || !value)
		return ;
	index = hash_function(key, table->size);
	item = table->items[index];
	while (item && ft_strcmp(item->key, key) != 0)
		item = item->next;
	if (item && update_item_value(item, value) == 0)
		return ;
	new_item = create_item(key, value);
	if (!new_item)
		return ;
	if (table->items[index] == NULL)
	{
		table->items[index] = new_item;
		table->count++;
	}
	else
		hashmap_handle_collision(table, index, new_item);
}
