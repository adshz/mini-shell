/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_insert.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:51:48 by szhong            #+#    #+#             */
/*   Updated: 2025/01/23 16:55:03 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"
#include "errors.h"

static void	hashmap_replace_item(t_hash_item *current, char *value,
		t_hash_item *new_item, int flag)
{
	free(current->value);
	current->value = NULL;
	current->value = ft_strdup(value);
	current->flag = flag;
	hashmap_free_item(new_item);
}

static int	hashmap_insert_item(t_hashmap *table, t_hash_item *new_item,
						unsigned long int index)
{
	if (table->count == table->size)
	{
		exit_handler(NULL, NULL, HASH_FULL, NOT_EXIT);
		hashmap_free_item(new_item);
		return (0);
	}
	table->items[index] = new_item;
	table->count++;
	return (1);
}
/**
 * @brief Insert or update a key-value pair in the hash table
 *
 * This function performs the following operations"
 * 1. validate input parameters (table, key, value)
 * 2. calculates hash index using hash_functin 
 * 3. checks if key already exists in the table:
 * 	- if exists: updates the value, optionally freeing the old value
 * 	- if not, create new item and adds it to the front of the chain
 * 4. handles memory allocation failures through the process
 *
 * @param table Pointer to the hash table
 * @param key Key string to insert/update
 * @param value Value string to assoicate with the key
 * @param free_old Flag indicating whether to free old value when updating (1)
 * or not (0)
 * @return 0 on success, -1 on failure (memory allocation error or invalid
 * parameter)
 *
 * @note env_to_hashtable() free_old is set to 1
 * @see env_to_hashtable()
 */
int	hashmap_insert(t_hashmap *table, char *key, char *value, int flag)
{
	unsigned long int	index;
	t_hash_item			  *new_item;
	t_hash_item			  *current;

	if (!table || !key || !value)
		return (HASH_ERR);
	index = hash_function(key, table->size);
	new_item = hashmap_create_item(key, value, flag);
	current = table->items[index];
	if (current == NULL)
	{
		if (!hashmap_insert_item(table, new_item, index))
			return (HASH_ERR);
	}
	else
	{
		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
			hashmap_replace_item(current, value, new_item, flag);
		else
			hashmap_handle_collision(table, index, new_item, flag);
	}
	return (HASH_OK);
}
