/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_string_helper.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 11:57:27 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 11:58:20 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

static int	validate_env_params(char **env_array, size_t *index,
		t_hash_item *item)
{
	if (!env_array || !index || !item)
	{
		ft_putendl_fd("Error: NULL pointer in add_env_item", 2);
		return (0);
	}
	if (*index >= HASH_SIZE)
	{
		ft_putendl_fd("Error: Index out of bounds in add_env_item", 2);
		return (0);
	}
	return (1);
}

static int	validate_env_item(t_hash_item *item)
{
	if (!item->key || !item->value)
	{
		ft_putendl_fd("Error: Invalid key/value in add_env_item", 2);
		return (0);
	}
	if (ft_strlen(item->key) == 0)
	{
		ft_putendl_fd("Error: Empty key in add_env_item", 2);
		return (0);
	}
	return (1);
}

static int	create_and_add_env_string(char **env_array, size_t *index,
		t_hash_item *item)
{
	char	*env_str;

	env_str = create_env_string(item->key, item->value);
	if (!env_str)
	{
		ft_putstr_fd("Error: Failed to create env string for key '", 2);
		ft_putstr_fd(item->key, 2);
		ft_putendl_fd("'", 2);
		cleanup_partial_array(env_array, *index);
		return (0);
	}
	env_array[*index] = env_str;
	(*index)++;
	return (1);
}

int	add_env_item(char **env_array, size_t *index, t_hash_item *item)
{
	if (!validate_env_params(env_array, index, item))
		return (0);
	if (!validate_env_item(item))
		return (0);
	return (create_and_add_env_string(env_array, index, item));
}
