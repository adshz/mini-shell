/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_item_processor.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:09:00 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 15:11:42 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

static t_hash_item	*get_next_valid_item(t_hash_item *current)
{
	t_hash_item	*next;

	next = current->next;
	if (!next)
		return (NULL);
	if (!is_pointer_valid(next) || next == current)
		return (NULL);
	return (next);
}

int	is_pointer_valid(void *ptr)
{
	if ((uintptr_t)ptr < 0x1000 || (uintptr_t)ptr > 0x7fffffffffff)
	{
		ft_putendl_fd("Error: Invalid pointer detected", 2);
		return (0);
	}
	return (1);
}

static int	is_hash_item_valid(t_hash_item *item)
{
	if (!item->key || !item->value)
	{
		ft_putendl_fd("Error: Corrupted hash item", 2);
		return (0);
	}
	return (1);
}

static int	process_single_item(char **env_array,
		size_t *index, t_hash_item *current)
{
	if (!is_pointer_valid(current))
		return (0);
	if (!is_hash_item_valid(current))
		return (0);
	if (!add_env_item(env_array, index, current))
	{
		ft_putendl_fd("Error: Failed to add env item", 2);
		return (0);
	}
	return (1);
}

int	process_bucket_items(char **env_array, size_t *index, t_hash_item *item)
{
	t_hash_item	*current;
	size_t		items_processed;

	if (!env_array || !index || !item)
	{
		ft_putendl_fd("Error: NULL pointer in process_bucket_items", 2);
		return (0);
	}
	items_processed = 0;
	current = item;
	while (current && items_processed < 1000)
	{
		if (!process_single_item(env_array, index, current))
		{
			cleanup_partial_array(env_array, *index);
			return (0);
		}
		current = get_next_valid_item(current);
		items_processed++;
	}
	if (items_processed >= 1000)
	{
		ft_putendl_fd("Error: Too many items in bucket", 2);
		cleanup_partial_array(env_array, *index);
		return (0);
	}
	return (1);
}
