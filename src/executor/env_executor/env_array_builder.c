/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_string_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:13:04 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 15:17:54 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "utils/utils.h"

static int	validate_env(t_hashmap *env)
{
	if (!env)
	{
		ft_putendl_fd("env is NULL", STDERR_FILENO);
		return (0);
	}
	if (!env->items)
	{
		ft_putendl_fd("env->items is NULL", STDERR_FILENO);
		return (0);
	}
	return (1);
}

static char	**allocate_env_array(size_t count)
{
	char	**env_array;

	if (count == 0)
	{
		env_array = malloc(sizeof(char *));
		if (!env_array)
		{
			ft_putendl_fd("malloc failed for empty array", STDERR_FILENO);
			return (NULL);
		}
		env_array[0] = NULL;
		return (env_array);
	}
	if (count > SIZE_MAX / sizeof(char *) - 1)
	{
		ft_putendl_fd("Array size overflow", STDERR_FILENO);
		return (NULL);
	}
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
	{
		ft_putendl_fd("malloc failed for env array", STDERR_FILENO);
		return (NULL);
	}
	return (env_array);
}

char	**create_env_array(t_hashmap *env)
{
	char	**env_array;
	size_t	count;

	if (!validate_env(env))
		return (NULL);
	count = hashmap_size(env);
	env_array = allocate_env_array(count);
	if (!env_array)
		return (NULL);
	if (!fill_env_array(env, env_array))
	{
		ft_putendl_fd("fill_env_array failed", STDERR_FILENO);
		ft_free_array(env_array);
		return (NULL);
	}
	return (env_array);
}
