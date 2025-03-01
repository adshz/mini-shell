/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:26:08 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 20:34:31 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./echo.h"

static int	check_option(char *str)
{
	int	i;

	i = 0;
	if (str[0] != '-')
		return (0);
	i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] != 'n')
			return (0);
		i++;
	}
	return (0);
}

int	builtin_echo(char **argv)
{
	int	i;
	int	opt;

	i = 1;
	opt = 0;
	while (argv[i] != NULL && check_option(argv[i]) == 1)
	{
		opt = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			ft_putstr_fd(" ", 1);
		i++;
	}
	if (opt == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
