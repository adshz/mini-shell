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

/**
 * @brief Creates a new hash table item
 *
 * Allocates and initialises a new hash table item with copies of the provided
 * key and value strings.
 *
 * @param key Key string to be copied
 * @param value Value string to be copied
 * @return t_hash_item* New item on success, NULL on any allocation failure
 *
 * @note Performs deep copy of both key and value
 * @note Handles cleanup if any allocation fails
 * @note Sets next pointer to NULL (for collision handling)
 */
static t_hash_item	*create_new_item(char *key, char *value)
{
	t_hash_item	*new_item;

	new_item = (t_hash_item *)malloc(sizeof(t_hash_item));
	if (!new_item)
		return (NULL);
	new_item->key = key;
	new_item->value = value;
	new_item->next = NULL;
	return (new_item);
}

/**
 * @brief Searches for an item with matching key in the hash table
 *
 * Traverses the linked list at the specified index to find an item with a
 * matching key
 *
 * @param table Hash table to search in
 * @param key Key to search for
 * @param index Index in the hash table to search
 * @return t_hash_item* Matching item if found, NULL if not found
 *
 * @note Uses ft_strcmp for key comparsion
 * @note Handles coolision chains through next pointer
 */
static t_hash_item	*find_existing_item(t_hashmap *table, \
									char *key, size_t index)
{
	t_hash_item	*item;

	item = table->items[index];
	while (item)
	{
		if (ft_strcmp(item->key, key) == 0)
			return (item);
		item = item->next;
	}
	return (NULL);
}

/**
 * @brief Updates an existing hash item's value
 *
 * Creates a new copy of the value and updates the item.
 * Optionally frees the old value based on free_old parameter
 *
 * @param item Target item in hash table to update
 * @param value New value to set for the target item
 * @param free_old Flag to determine if old value should be freed
 * @return int 0 on success, -1 on memory allocation failure
 *
 * @note Only frees old value if free_old is truly value; When updating
 * environment variables from our t_shell struct, we own this data so we need to
 * free it; When we don't own the data, we don't free memory. see code example
 * below
 * @note New value is always duplicated for memory safety, data independence,
 * cocurrent modification control, lifetime management
 * 
 * Example:
 * @code
 *  // When we don't own the data
 *  const char* system_path = "/usr/bin";
 *  hashmap_insert(env, "PATH", system_path, 0);
 *
 *  // Memory handling is elsewhere
 *  char *home = getenv("HOME");
 *  hashmap_insert(table1, "HOME", 0);
 *  hashmap_insert(table2, "HOME", 0);
 * @endcode
 */
static int	update_existing_item(t_hash_item *item, char *value, int free_old)
{
	char	*new_value;

	new_value = ft_strdup(value);
	if (!new_value)
		return (HASH_ERR);
	if (free_old)
		free(item->value);
	item->value = new_value;
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
int	hashmap_insert(t_hashmap *table, char *key, char *value, int free_old)
{
	size_t		index;
	t_hash_item	*existing;
	t_hash_item	*new_item;

	if (!table || !key || !value)
		return (HASH_ERR);
	index = hash_function(key, table->size);
	existing = find_existing_item(table, key, index);
	if (existing)
		return (update_existing_item(existing, value, free_old));
	new_item = create_new_item(key, value);
	if (!new_item)
		return (HASH_ERR);
	new_item->next = table->items[index];
	table->items[index] = new_item;
	return (HASH_OK);
}
