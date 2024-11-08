/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:42:19 by szhong            #+#    #+#             */
/*   Updated: 2024/11/08 14:45:55 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include "shell.h"

int	main(void)
{
	char	*str = "Hello world";
	int		len = test_drive(str);

	ft_printf("works if it is %d", len);
	return (0);
}
