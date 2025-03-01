/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_insert.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 16:51:48 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:32:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"
#include "errors.h"

static void	hashmap_replace_item(t_shell *shell, t_hash_item *current, \
								char *value, t_hash_item *new_item)
{
	char	*new_value;

	new_value = ft_strdup(new_item->value);
	if (!new_value)
		return ;
	(void)ft_memory_collector(shell, new_value, false);
	ft_memory_delone(shell, current->value);
	hashmap_free_item(shell, &new_item);
}

static int	hashmap_insert_item(t_shell *shell, t_hashmap *table, \
							t_hash_item *new_item, unsigned long int index)
{
	if (table->count == table->size)
	{
		exit_handler(NULL, NULL, HASH_FULL, NOT_EXIT);
		hashmap_free_item(shell, &new_item);
		return (HASH_ERR);
	}
	table->items[index] = new_item;
	table->count++;
	return (HASH_OK);
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
t_hash_item	*hashmap_insert(t_shell *shell, t_hashmap_insert_params params)
{
	unsigned long int		index;
	t_hash_item				*new_item;
	t_hash_item				*current;

	if (!params.key)
		return (NULL);
	index = hash_function(params.key, params.table->size);
	new_item = hashmap_create_item(shell, params.key, \
								params.value, params.flag);
	current = params.table->items[index];
	if (current == NULL)
	{
		if (!hashmap_insert_item(shell, params.table, new_item, index))
			return (NULL);
	}
	else
	{
		if (ft_strncmp(current->key, params.key, ft_strlen(params.key) + 1) == 0)
			hashmap_replace_item(shell, current, params.value, new_item, params.flag);
		else
			hashmap_handle_collision(params.table, index, new_item, params.flag);
	}
	return (NULL);
}
// To put this into libft later on.
// t_hash_item	*hashmap_insert(t_shell *shell, t_hashmap *table, char *key, char *value, int flag)
// {
// 	unsigned long int		index;
// 	t_hash_item				*new_item;
// 	t_hash_item				*current;

// 	if (!key)
// 		return (NULL);
// 	index = hash_function(key, table->size);
// 	new_item = ft_memory_collector(shell, \
// 								hashmap_create_item(key, value, flag), false);
// 	current = table->items[index];
// 	if (current == NULL)
// 	{
// 		if (!hashmap_insert_item(table, new_item, index))
// 			return (NULL);
// 	}
// 	else
// 	{
// 		if (ft_strncmp(current->key, key, ft_strlen(key) + 1) == 0)
// 			hashmap_replace_item(current, value, new_item, flag);
// 		else
// 			hashmap_handle_collision(table, index, new_item, flag);
// 	}
// 	return (NULL);
// }
