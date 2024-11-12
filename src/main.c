/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:42:19 by szhong            #+#    #+#             */
/*   Updated: 2024/11/11 15:37:54 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>

int	main(void)
{
	char	*usrin;

	while (1)
	{
		usrin = readline(">>minishell$: ");
		if (usrin[0] != '\0')
			add_history(usrin);
		if (ft_strcmp(usrin, "exit") == 0)
		{
			free(usrin);
			break ;
		}
		else if (ft_strcmp(usrin, "pwd") == 0)
		{
			if (ft_pwd() != 0)
			{
				ft_putstr_fd("Error: ft_pwd", 2);
				break ;
			}

		}
		free(usrin);
	}
	rl_clear_history();
	return (0);
}
