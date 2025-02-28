/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_replace_value.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 21:26:01 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 21:26:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

void	hashmap_replace_value(t_shell *shell, t_hashmap *table, \
							const char *key, const char *new_value)
{
	t_hash_item	*item;

	if (!table || !key || !value)
		return ;
	item = hashmap_get_item(table, key);
	if (!item)
		return ;
	ft_memory_delone(shell, item->value);
	item->value = ft_memory_collector(shell, ft_strdup(new_value), false);
}
