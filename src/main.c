/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:55:46 by szhong            #+#    #+#             */
/*   Updated: 2025/02/25 05:06:42 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"

int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;

	if (argc != 1)
		exit_handler(INV_ARGS);
	init_shell(&shell, argv, envp);
	interactive_loop(&shell);
	cleanup_minishell(&shell);
	return (shell.exit_status);
}
