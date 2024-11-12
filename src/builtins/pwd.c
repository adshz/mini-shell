/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 19:43:31 by szhong            #+#    #+#             */
/*   Updated: 2024/11/12 20:04:00 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"
#include "libft.h"
#include <unistd.h>

char*	ft_pwd(void)
{
	char	*cdir;

	cdir = getcwd(NULL, 0);
	if (cdir == NULL)
		ft_putstr_fd("Error: getcwd", 2);
	ft_printf("%s\n", cdir);
	//free(cdir);
	return (cdir);
}

