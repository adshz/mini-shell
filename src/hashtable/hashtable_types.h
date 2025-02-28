/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_types.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 12:12:30 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 12:12:53 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HASHTABLE_TYPES_H
# define HASHTABLE_TYPES_H

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
	int					flag;
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

typedef struct s_hashmap_insert_params
{
	t_hashmap	*table;
	char	*key;
	char	*value;
	int		flag;
}	t_hashmap_insert_params;
#endif

