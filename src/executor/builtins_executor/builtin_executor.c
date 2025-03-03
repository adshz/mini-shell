/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:44:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 20:20:19 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	execute_builtin(t_shell *shell, char **argv)
{
	if (!argv[0])
		return (1);
	if (ft_strcmp(argv[0], "echo") == 0)
		return (builtin_echo(argv));
	if (ft_strcmp(argv[0], "cd") == 0)
		return (builtin_cd(shell, argv[1]));
	if (ft_strcmp(argv[0], "pwd") == 0)
		return (builtin_pwd());
	if (ft_strcmp(argv[0], "export") == 0)
		return (builtin_export(shell, argv));
	if (ft_strcmp(argv[0], "unset") == 0)
		return (builtin_unset(shell, argv));
	if (ft_strcmp(argv[0], "env") == 0)
		return (builtin_env(shell));
	builtin_exit(shell, argv);
	return (ERRNO_GENERAL);
}

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
			return (true);
		i++;
	}
	return (false);
}
