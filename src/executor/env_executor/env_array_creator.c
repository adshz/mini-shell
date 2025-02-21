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

int	iterate_hash_buckets(t_hashmap *env, char **env_array)
{
	size_t	array_index;

	if (!validate_hashmap(env, env_array))
		return (0);
	array_index = 0;
	return (process_all_hash_buckets(env, env_array, &array_index));
}

int	fill_env_array(t_hashmap *env, char **env_array)
{
	if (!env || !env_array)
	{
		ft_putendl_fd("Error: NULL pointer in fill_env_array", 2);
		return (0);
	}
	if (!iterate_hash_buckets(env, env_array))
	{
		cleanup_partial_array(env_array, hashmap_size(env));
		return (0);
	}
	return (1);
}
