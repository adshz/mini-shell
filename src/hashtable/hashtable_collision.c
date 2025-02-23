/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_collision.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:22:13 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/22 11:57:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "hashtable.h"

void	hashmap_handle_collision(t_hashmap *table, size_t index,
				t_hash_item *new_item, int flag)
{
	t_hash_item	*current;

	current = table->items[index];
	while (current->next != NULL)
		current = current->next;
	current->next = new_item;
	current->next->flag = flag;
	current->next->next = NULL;
	table->count++;
}
