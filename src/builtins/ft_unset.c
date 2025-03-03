/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 22:23:22 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 23:05:43 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

int	builtin_unset(t_shell *shell, char **argv)
{
	int	i;
	bool	err;
	char	*key;

	i = 1;
	if (!argv[1])
		return (0);
	err = false;
	while (argv[i])
	{
		if (!check_valid_key(argv[i]) == 0)
		{
			ft_putstr_fd("minishell: unset: '", 2);
			ft_putstr_fd(argv[i], 2);
			ft_putstr_fd(": not a valid identifier\n", 2);
			err = true;
		}
		else
		{
			key = ft_memory_collector(shell, dup_key(shell, argv[1]), false);
			hashmap_remove(shell, shell->env, key);
		}
		i++;
	}
	return (err);
}
