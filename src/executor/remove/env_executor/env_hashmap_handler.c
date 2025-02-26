/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_hashmap_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:58:52 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 15:07:43 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

// iterate_hash_buckets -> validate_hashmap
// Validate the hashmap and the env_array
int	validate_hashmap(t_hashmap *env, char **env_array)
{
	if (!env || !env_array || !env->items)
	{
		ft_putendl_fd("Error: NULL pointer in iterate_hash_buckets", 2);
		return (0);
	}
	if (env->size == 0 || env->size > HASH_SIZE)
	{
		ft_putendl_fd("Error: Invalid hash table size", 2);
		return (0);
	}
	return (1);
}

static int	process_hash_bucket(char **env_array, size_t *array_index,
		t_hash_item *bucket)
{
	if (bucket && !process_bucket_items(env_array, array_index, bucket))
	{
		ft_putendl_fd("Error: Failed to process bucket", 2);
		return (0);
	}
	return (1);
}

// iterate_hash_buckets -> process_all_hash_buckets
int	process_all_hash_buckets(t_hashmap *env,
		char **env_array, size_t *array_index)
{
	size_t	i;

	i = 0;
	*array_index = 0;
	while (i < env->size)
	{
		if (!process_hash_bucket(env_array, array_index, env->items[i]))
		{
			cleanup_partial_array(env_array, *array_index);
			free(env_array);
			return (0);
		}
		i++;
	}
	env_array[*array_index] = NULL;
	return (1);
}
