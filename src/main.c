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
#include "mem_tracker.h"
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

/**
 * @brief Validates command line arguments for the shell
 *
 * Ensures the shell is started with correct number of arguments.
 * Exits with error code 127 if more than one argument is provided.
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 *
 * @note Shell only accepts program name as argument
 * @note Exits program if validation fails
 */
static void	validate_args(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": Input Many Arguments Before Program", STDERR_FILENO);
		exit(127);
	}
}

/**
 * @brief Entry point for the minishell program
 *
 * Program flow:
 * - Validates command line arguments
 * - Initializes shell environment and resources
 * - Enters interactive command loop
 * - Performs cleanup before exit
 *
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @param envp Array of environment variables
 * @return Exit status of last executed command
 *
 * @note Exits with ERROR if shell initialization fails
 * @see init_shell() for shell initialization
 * @see interactive_loop() for main shell loop
 * @see cleanup_shell() for resource cleanup
 */
int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;
	int		exit_status;

	validate_args(argc, argv);
	if (init_shell(&shell, argv, envp) != SHELL_SUCCESS)
	{
		ft_putendl_fd("Shell Initialisation Failure", STDERR_FILENO);
		exit(SHELL_ERROR);
	}
	interactive_loop(&shell);
	exit_status = shell.exit_status;
	rl_clear_history();
	cleanup_shell(&shell);
	return (exit_status);
}
