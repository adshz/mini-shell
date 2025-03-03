/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_deleter.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:33:27 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:33:53 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "memory_collector.h"

void	ft_delete_mem(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	*ft_memory_delone(t_shell *shell, void *ptr)
{
	t_list	**current;
	t_list	*tmp;

	if (!shell || !ptr || !shell->mem_list)
		return (NULL);
	current = &shell->mem_list;
	while (*current)
	{
		if ((*current)->content == ptr)
		{
			tmp = *current;
			*current = (*current)->next;
			ft_lstdelone(tmp, ft_delete_mem);
			return (NULL);
		}
		current = &(*current)->next;
	}
	return (NULL);
}
