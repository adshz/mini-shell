/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 20:53:34 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 20:54:24 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "signals/ft_signals.h"

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

void	config_execution_signals(void)
{
	g_signal_status = SHELL_STATE_EXECUTION;
	signal(SIGQUIT, SIG_IGN);
}

static void	main_sigint_handler(int signum)
{
	(void)signum;
	if (g_signal_status == SHELL_STATE_HEREDOC_INPUT)
	{
		close(STDIN_FILENO);
		ft_putstr_fd("\n", 1);
		g_signal_status = SHELL_STATE_HEREDOC_INTERRUPTED;
	}
	else if (g_signal_status == SHELL_STATE_READLINE)
	{
		ft_putstr_fd("\n", 1);
		rl_replace_line("", 0);
		rl_on_new_line();
		rl_redisplay();
	}
}

static void	disable_ctrl_char_echo(void)
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
	term.c_oflag |= OPOST;  // Enable output processing
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1)
	{
		perror("tcgetattr");
		tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
		return ;
	}
}

void	config_main_signals(void)
{
	g_signal_status = SHELL_STATE_READLINE;
	disable_ctrl_char_echo();
	signal(SIGINT, main_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
