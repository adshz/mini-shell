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
#include "types.h"

int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;

	if (arg != 1)
		exit_hanlder(NULL, argv[1], INV_ARGS, IS_EXIT)
	init_shell(&shell, argv, envp)
	interactive_loop(&shell);
	rl_clear_history();
	cleanup_shell(&shell);
	return (shell.exit_status);
}
