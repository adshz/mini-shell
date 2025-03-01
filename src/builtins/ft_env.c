/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 21:25:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 21:35:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

int	builtin_env(t_shell *shell)
{
	t_hash_item	*current;
	size_t		index;

	index = 0;
	while (index < shell->env->size)
	{
		current = shell->env->items[index];
		while (current != NULL)
		{
			ft_printf("%s=%s\n", current->key, current->value);
			current = current->next;
		}
		index++;
	}
	return (ERRNO_NONE);
}

