/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_item_processor_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:47:36 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:47:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_hash_item	*get_next_valid_item(t_hash_item *current)
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

int	is_hash_item_valid(t_hash_item *item)
{
	if (!item->key || !item->value)
	{
		ft_putendl_fd("Error: Corrupted hash item", 2);
		return (0);
	}
	return (1);
}

int	process_single_item(char **env_array,
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
