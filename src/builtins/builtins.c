/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:36:50 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:04:07 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins.h"

static const char	**get_builtins(void)
{
	static const char	*builtins[8];

	builtins[0] = "echo";
	builtins[1] = "cd";
	builtins[2] = "pwd";
	builtins[3] = "export";
	builtins[4] = "unset";
	builtins[5] = "env";
	builtins[6] = "exit";
	builtins[7] = NULL;
	return (builtins);
}

bool	is_builtin(const char *cmd)
{
	int			i;
	const char	**builtins;

	if (!cmd)
		return (false);
	i = 0;
	builtins = get_builtins();
	while (builtins[i])
	{
		if (ft_strcmp((char *)cmd, (char *)builtins[i]) == 0)
		{
			if (ft_strcmp(cmd, "env") == 0)
				return (false);
			return (true);
		}
		i++;
	}
	return (false);
}
