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

static int	validate_bucket_inputs(char **env_array, size_t *index,
		t_hash_item *item)
{
	if (!env_array || !index || !item)
	{
		ft_putendl_fd("Error: NULL pointer in process_bucket_items", 2);
		return (0);
	}
	return (1);
}

static int	process_bucket_loop(char **env_array, size_t *index,
		t_hash_item *current)
{
	size_t	items_processed;

	items_processed = 0;
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

int	process_bucket_items(char **env_array, size_t *index, t_hash_item *item)
{
	t_hash_item	*current;

	if (!validate_bucket_inputs(env_array, index, item))
		return (0);
	current = item;
	if (!process_bucket_loop(env_array, index, current))
	{
		cleanup_partial_array(env_array, *index);
		return (0);
	}
	return (1);
}
