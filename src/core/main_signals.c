/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_signals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:16:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/25 07:08:37 by szhong           ###   ########.fr       */
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

static void	main_sigint_handler(int signum)
{
	(void)signum;
	if (g_signal_status == SHELL_STATE_HEREDOC_INPUT)
	{
		close(STDIN_FILENO);
		ft_putstr_fd("\n", 1);
		g_signal_status == SHELL_STATE_HEREDOC_INTERRUPTED;
	}
	else if (g_signal_status == SHELL_STATE_READLINE)
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

void	config_main_signals(t_shell *shell)
{
	struct termios	term;

	g_signal_status = SHELL_STATE_READLINE;
	term = shell->original_term;
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
	signal(SIGINT, main_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
