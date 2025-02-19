/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:57:55 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:25:03 by evmouka          ###   ########.fr       */
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
// Signal states for global signal handling
# define SIG_NONE 0          // No signal/normal operation
# define SIG_HEREDOC_MODE 1  // Currently in heredoc mode
# define SIG_HEREDOC_INT 2   // Heredoc was interrupted
# define SIG_REGULAR_INT 3   // Regular interrupt
# include "shell.h"
# include <signal.h>
# include "lexer/lexer.h"
/**
 * 
 * Without extern, each file would create its own separate variable 
 * instead of using the one defined in main.c
 */
extern volatile sig_atomic_t	g_signal_status;

/* Signal handling functions */
void	setup_signals(void);
void	setup_terminal_settings(void);
void	disable_ctrl_char_echo(void);
void	restore_signal_handlers(void);

/* Shell initialization functions */
int		init_terminal(t_shell *shell);
int		init_environment(t_shell *shell, char **envp);
int		init_env(t_shell *shell, char **envp);
int		init_io(t_shell *shell);
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
int		init_shell(t_shell *shell, char *argv[], char *envp[]);

/* AST building and parsing */
/**
 * @brief Parses command line and builds abstract syntax tree
 *
 * @param shell Pointer to shell structure
 * @return true if parsing and AST building succeed, false otherwise
 */
bool	parse_and_build_ast(t_shell *shell);

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

/* Signal handling */
/*
 * @brief Initializes signal handlers for the shell
 *
 * Sets up handlers for:
 * - SIGINT (Ctrl+C)
 * - SIGQUIT (Ctrl+\)
 * - Other relevant signals
 */
void	init_signals(void);
void	handle_sigint(int sig);

#endif
