/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_creator.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:11:42 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:14:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "hashtable/hashtable.h"

int	add_env_item(char **env_array, size_t *index, t_hash_item *item)
{
	if (!item->key || !item->value)
		return (1);
	env_array[*index] = create_env_string(item->key, item->value);
	if (!env_array[*index])
		return (0);
	(*index)++;
	return (1);
}

int	process_bucket_items(char **env_array, size_t *index, t_hash_item *item)
{
	while (item)
	{
		if (!add_env_item(env_array, index, item))
			return (0);
		item = item->next;
	}
	return (1);
}

int	iterate_hash_buckets(t_hashmap *env, char **env_array)
{
	size_t	i;
	size_t	array_index;

	i = 0;
	array_index = 0;
	while (i < env->size)
	{
		if (!process_bucket_items(env_array, &array_index, env->items[i]))
			return (0);
		i++;
	}
	env_array[array_index] = NULL;
	return (1);
}

int	fill_env_array(t_hashmap *env, char **env_array)
{
	if (!env || !env_array)
		return (0);
	return (iterate_hash_buckets(env, env_array));
}
