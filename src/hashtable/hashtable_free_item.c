/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_free_item.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 10:29:08 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:32:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

void	hashmap_free_item(t_shell *shell, t_hash_item **item_ptr)
{
	t_hash_item	*item;

	if (!item_ptr || !*item_ptr)
		return ;
	item = *item_ptr;
	ft_memory_delone(shell, item->key);
	ft_memory_delone(shell, item->value);
	ft_memory_delone(shell, item);
	*item_ptr = NULL;
}
