/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 15:31:16 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 12:00:51 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HASHTABLE_H
# define HASHTABLE_H

# define HASH_OK 0
# define HASH_ERR 1

# define HASH_SIZE 1024

# include "libft.h"
# include <stdlib.h>

/* Hash Table - Hash Item Structure */
/**
 * @brief Hash Table Item Structure
 *
 * Represents a single key-value pair in the hash table,
 * with support for collision resolution through chaining
 *
 * @param key   Environment variable name (e.g., "PATH", "HOME")
 * @param value Environment variable value (e.g., "/usr/bin:/bin")
 * @param next  Pointer to next item in case of hash collision
*/
typedef struct s_hash_item
{
	char				*key;
	char				*value;
	struct s_hash_item	*next;
}	t_hash_item;

typedef struct s_hash_count
{
	size_t		count;
	size_t		i;
	t_hash_item	*current;
	size_t		max_items_per_bucket;
	size_t		items_in_bucket;
}	t_hash_count;

/* Hash Table Structure */
/**
 * @brief Hash table structure for environment variables
 *
 * Manages environment variables using a hash table implementation.
 * Provides O(1) average case access time for variable lookups
 *
 * @param items Array of hash item pointers (the hash table buckets)
 * @param size  Number of buckets in the hash table
 * @param count Number of items currently stored in the table
 */
typedef struct s_hashmap
{
	t_hash_item	**items;
	size_t		size;
	size_t		count;
}	t_hashmap;

/* Function Prototypes */
t_hashmap	*hashmap_create_table(size_t size);
/**
 * @brief Creates a new hash table with default size
 * @return Pointer to new hash table, NULL if allocation fails
 */
t_hashmap	*hashmap_create(void);

/**
 * @brief Converts environment array to hash table
 * @param envp Array of environment strings in "key=value" format
 * @return Pointer to new hash table, NULL if conversion fails
 */
t_hashmap	*env_to_hashtable(char **envp);

/**
 * @brief Removes an item from the hash table
 * @param table Hash table to remove from
 * @param key Key of item to remove
 */
void		hashmap_remove(t_hashmap *table, const char *key);
/**
 * @brief Destroys hash table and frees all memory
 * @param table Hash table to destroy
 */
void		hashmap_destroy(t_hashmap *table);
/**
 * @brief Inserts a key-value pair into the hash table
 * @param table Hash table to insert into
 * @param key Key to insert
 * @param value Value to insert
 * @param update_existing Flag to update existing key if it exists
 * @return 1 if key was inserted, 
 * 0 if key already exists and update_existing is false
 */
int			hashmap_insert(t_hashmap *table, char *key, \
				char *value, int free_old);
/**
 * @brief Gets a value from the hash table
 * @param table Hash table to get from
 * @param key Key to get
 * @return Value associated with key, NULL if not found
 */
char		*hashmap_get(t_hashmap *table, const char *key);
/**
 * @brief Searches for a value in the hash table
 * @param table Hash table to search in
 * @param key Key to search for
 * @return Value associated with key, NULL if not found
 */
char		*hashmap_search(t_hashmap *table, char *key);
void		hashmap_iterate(t_hashmap *table, \
					void (*f)(const char *, const char *));
void		hashmap_set(t_hashmap *table, const char *key, const char *value);
void		hashmap_destroy(t_hashmap *table);
void		hashmap_handle_collision(t_hashmap *table, size_t index, \
							t_hash_item *new_item);
size_t		hashmap_size(t_hashmap *table);
size_t		hash_function(const char *key, size_t table_size);
#endif
