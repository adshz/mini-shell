/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:16:12 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 16:41:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell.h"
#include <signal.h>
#include <termios.h>
#include <unistd.h>

extern t_shell *g_shell;
/**
 * @brief Disables the echoing of control characters in terminal
 *
 * Modifies terminal settings to prevent control characters (like ^C)
 * from being displayed when signals are triggered.
 * Uses termios structure to modify terminal attributes.
 */
static void	disable_ctrl_char_echo(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

/**
 * @brief Signal handler for SIGINT (Ctrl+C)
 *
 * Handles interrupt signal by:
 * 1. Setting global signal flag
 * 2. Printing newline
 * 3. Clearing and redisplaying prompt if in interactive mode
 *
 * @param signum Signal number (SIGINT)
 *
 * @note Uses readline functions to handle prompt redisplay
 * @note Only redraws prompt if running in interactive terminal
 */
void	handle_sigint(int sig)
{
	(void)sig;
	g_shell->signal = SIGINT;
	write(STDERR_FILENO, "\n", 1);
	if (g_shell->in_heredoc)
	{
		g_shell->heredoc_sigint = true;
		close(STDIN_FILENO);
		return;
	}
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Initializes signal handling for the shell
 *
 * Sets up signal handlers for:
 * - SIGINT (Ctrl+C): Custom handler for showing a newline,redisplaying prompt
 * - SIGQUIT (Ctrl+\): Ignored in interactive mode
 *
 * Also:
 * - Disables control character echo
 * - Initializes global signal flag
 * - Uses SA_RESTART to automatically restart interrupted system calls
 *
 * @note Uses sigaction for reliable signal handling
 * @note Global signal flag is used for signal state tracking
 * @note Ctrl+D (EOF) handling is not done here as it's not a signal
 *       It's handled in interactive_loop through readline() return value
 */
void	init_signals(void)
{
	struct sigaction	sa;

	g_shell->signal = 0;
	disable_ctrl_char_echo();
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
	rl_catch_signals = 0;
}
