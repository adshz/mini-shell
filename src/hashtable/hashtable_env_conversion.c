/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_env_conversion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:06:12 by szhong            #+#    #+#             */
/*   Updated: 2025/01/23 16:22:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"
#include "errors.h"

/**
 * @brief Process a single environment variable string and add it to hashtable
 *
 * Take a string in the format "KEY=VALUE" and splits it into key-value pair
 * for insertion into the hash table for managing environment variable.
 * Handle memory allocation and cleanup for both successful and failed
 * operations
 *
 * @param env_str String in format "KEY=VALUE" to be processed
 * @param env Target hash table for environement variable 
 * @return SUCCESS(0) on success, ERROR (1) on faliure
 *
 * @note Free all allocated memory on failure
 * @note Return 0 for entries without '=' to skip. Because environement varibale
 * has to be in the format 'KEY=VALUE' if not it is not valid
 * @note Return 0 when skipping invalid environment variables; because
 * invalid environment variable is not an error conditions.   
 */
static int	process_env_entry(char *env_str, t_hashmap *env)
{
	char	*equals_pos;
	char	*key;
	char	*value;
	size_t	key_len;

	equals_pos = ft_strchr(env_str, '=');
	if (!equals_pos)
		return (HASH_OK);
	key_len = equals_pos - env_str;
	key = ft_substr(env_str, 0, key_len);
	value = ft_strdup(equals_pos + 1);
	if (!key || !value || hashmap_insert(env, key, value, 0) != HASH_OK)
	{
		free(key);
		free(value);
		return (HASH_ERR);
	}
	return (HASH_OK);
}

/**
 * @brief Converts environment array to hash table
 *
 * Creates a new hash table and populates it with environment variables.
 * Each string in envp must be in "KEY=VALUE" format. Otherwise, it is invalid.
 * Caller is responsible for freeing returned t_hashmap using hashmap_destory()
 *
 * @param envp C-like NULL-terminated array of environment strings
 * @return t_hashmap* Pointer to t_hashmap type variable, hash table containing
 * environment variable, NULL on any failure
 *
 * Example
 * @code
 * char *envp[] = {"PATH=/bin", "HOME=/home/user", NULL};
 * t_hashmap *env = env_to_hashtable(envp);
 * if (!envp)
 *   return (-1);
 * @endcode
 */
t_hashmap	*env_to_hashtable(char *envp[])
{
	t_hashmap	*env;
	int			i;

	env = hashmap_create_table(HASH_SIZE);
	if (!envp || !env)
	{
		ft_putendl_fd("Hashing Environment Table Failure!", STDERR_FILENO);
		return (NULL);
	}
	i = 0;
	while (envp[i])
	{
		if (process_env_entry(envp[i], env) != 0)
		{
			ft_putendl_fd("Process Environement Variable Failure!", \
				STDERR_FILENO);
			hashmap_destroy(env);
			return (NULL);
		}
		i++;
	}
	return (env);
}
