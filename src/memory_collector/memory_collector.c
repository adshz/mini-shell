/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_collector.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:32:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:33:09 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "memory_collector.h"

void	*ft_memory_collector(t_shell *shell, void *ptr, bool clean_flag)
{
	if (clean_flag)
	{
		ft_lstclear(&shell->mem_list, ft_delete_mem);
		return (NULL);
	}
	else
	{
		ft_lstadd_back(&shell->mem_list, ft_lstnew(ptr));
		return (ptr);
	}
}
