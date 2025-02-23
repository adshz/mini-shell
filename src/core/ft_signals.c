/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:16:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:28:50 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include <signal.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>

// extern is a keyword that is used to declare a variable that is defined
// outside of the current file.
// volatile is a keyword that is used to declare a variable that can be changed
// by the program or by the operating system.
// sig_atomic_t is a keyword that is used to declare a variable that can be
// changed by the program or by the operating system.
// g_signal_status is a variable that is used to store the signal status.
// it is declared as volatile because it can be changed by the program or
// by the operating system.
/**
 * 
 * Without extern, each file would create its own separate variable 
 * instead of using the one defined in main.c
 */
extern volatile sig_atomic_t	g_signal_status;

/**
 * @brief Disables the echoing of control characters in terminal
 *
 * Modifies terminal settings to prevent control characters (like ^C)
 * from being displayed when signals are triggered.
 * Uses termios structure to modify terminal attributes.
 */
void	disable_ctrl_char_echo(void)
{
	struct termios	term;
	struct termios	old_term;

	if (tcgetattr(STDIN_FILENO, &term) == -1)
	{
		perror("tcgetattr");
		return ;
	}
	old_term = term;
	term.c_lflag &= ~(ECHOCTL | ICANON);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcsetattr");
		tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
		return ;
	}
}

void	ft_sigquit_handler(int signum)
{
	(void)num;
	ft_putstr_fd("Quit: 3", 1);
}

static void	handle_signint_input(int signum)
{
	g_signal = signum;
	ioctl(STDIN_FILENO, TIOCSTI, "\n");
	rl_replace_line("", 0);
	rl_on_new_line();
}

void	restore_signal_handlers(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sigint;
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}

void	config_signals_input(void)
{
	signal(SIGNINT, handle_sign_input);
	signal(SIGQUIT, SIG_IGN);

}
