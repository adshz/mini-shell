/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:57:55 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:55:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @file core.h
 * @brief Core functionality for the minishell program
 *
 * This header defines the core functions responsible for:
 * - Shell initialization and cleanup
 * - Command parsing and AST building
 * - Interactive shell loop
 * - Signal handling
 */
#ifndef CORE_H
# define CORE_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <errno.h>
# include <string.h>
# include <termios.h>
# include "shell.h"
# include <signal.h>
# include "lexer/lexer.h"
/**
 * 
 * Without extern, each file would create its own separate variable 
 * instead of using the one defined in main.c
 */
extern volatile sig_atomic_t	g_signal_status;
/* Shell initialization functions */
void	init_env_vars(t_shell *shell, char **argv);

/* Core structure initialisation */
/**
 * @brief Initializes the shell structure and environment
 *
 * @param shell Pointer to shell structure to initialize
 * @param argv  Array of command line arguments
 * @param envp  Array of environment variables
 * @return SUCCESS if initialization succeeds, ERROR otherwise
 */
void		init_shell(t_shell *shell, char *argv[], char *envp[]);

/**
 * @brief Main shell interactive loop
 *
 * Handles:
 * - Reading user input
 * - Parsing commands
 * - Executing commands
 * - Managing command history
 *
 * @param shell Pointer to shell structure
 */
void	interactive_loop(t_shell *shell);
#endif
