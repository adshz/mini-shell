/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 15:55:46 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 16:20:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"
#include "types.h"

/**
 * @brief Signal handler for handling signals
 *
 * This function is used to handle signals in the shell.
 * It sets the signal status to the signal number.
 * 
 * Without extern, each file would create its own separate variable 
 * instead of using the one defined in main.c
 */
volatile sig_atomic_t	g_signal_status = 0;


int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;
	int		exit_status;

	if (arg != 1)
		exit_hanlder(NULL, argv[1], INV_ARGS, IS_EXIT)
	init_shell(&shell, argv, envp)
	interactive_loop(&shell);
	exit_status = shell.exit_status;
	rl_clear_history();
	cleanup_shell(&shell);
	return (exit_status);
}
