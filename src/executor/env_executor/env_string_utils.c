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

#include <stdio.h>    // for fprintf, stderr
#include <stdint.h>   // for SIZE_MAX
#include "../executor.h"  // for hashmap_size, fill_env_array

char	*create_env_string(const char *key, const char *value)
{
	char	*env_str;
	size_t	key_len;
	size_t	value_len;
	size_t	total_len;

	if (!key || !value)
	{
		ft_putendl_fd("NULL key or value in create_env_string", STDERR_FILENO);
		return (NULL);
	}
	key_len = ft_strlen(key);
	value_len = ft_strlen(value);
	if (key_len > SIZE_MAX - 2 || key_len + 2 > SIZE_MAX - value_len)
	{
		ft_putendl_fd("String length overflow in create_env_string", STDERR_FILENO);
		return (NULL);
	}
	total_len = key_len + value_len + 2;
	env_str = malloc(total_len);
	if (!env_str)
	{
		ft_putendl_fd("malloc failed in create_env_string", STDERR_FILENO);
		return (NULL);
	}
	ft_strlcpy(env_str, key, key_len + 1);
	env_str[key_len] = '=';
	ft_strlcpy(env_str + key_len + 1, value, value_len + 1);
	return (env_str);
}

char	**create_env_array(t_hashmap *env)
{
	char	**env_array;
	size_t	count;

	if (!env)
	{
		ft_putendl_fd("env is NULL", STDERR_FILENO);
		return (NULL);
	}
	if (!env->items)
	{
		ft_putendl_fd("env->items is NULL", STDERR_FILENO);
		return (NULL);
	}
	count = hashmap_size(env);
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
	if (!fill_env_array(env, env_array))
	{
		ft_putendl_fd("fill_env_array failed", STDERR_FILENO);
		ft_free_array(env_array);
		return (NULL);
	}
	return (env_array);
}
