/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 21:37:28 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 21:38:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

int	built_pwd(void)
{
	char	*cwd;

	cwd = NULL;
	cwd = getcwd(cwd, 0);
	if (!cwd)
		return (1);
	ft_putendl_fd(cwd, 1);
	return (0);
}

