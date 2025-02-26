/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_create_item.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:52:39 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:21:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"

t_hash_item	*hashmap_create_item(t_shell *shell, char *key, char *value, int flag)
{
	t_hash_item	*new_item;

	new_item = \
		ft_memory_collector(shell, \
								(t_hash_item *)ft_calloc(1, \
								sizeof(t_hash_item)), false);
	if (!new_item)
		return (NULL);
	new_item->key = ft_memory_collector(shell, ft_strdup(key), false);
	new_item->value = ft_memory_collector(shell, ft_strdup(value), false);
	new_item->flag = flag;
	new_item->next = NULL;
	return (new_item);
}
