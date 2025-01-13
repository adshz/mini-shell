/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:21 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:56:21 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <signal.h>
#include <unistd.h>
#include <termios.h>
#include "shell.h"
#include <readline/readline.h>
#include <readline/history.h>

int g_signal;

static void disable_ctrl_char_echo(void)
{
	struct termios term;
	
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;  // Disable control char echo
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

static void	handle_sigint(int signum)
{
	ft_putendl_fd("\n[DEBUG] SIGINT received", STDERR_FILENO);
	g_signal = signum;

	// Always print a newline
	write(STDOUT_FILENO, "\n", 1);

	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	init_signals(void)
{
	struct sigaction sa;

	ft_putendl_fd("[DEBUG] Initializing signals", STDERR_FILENO);
	disable_ctrl_char_echo();
	
	sa.sa_handler = handle_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);

	// Reset global signal flag
	g_signal = 0;
	ft_putendl_fd("[DEBUG] Signal initialization complete", STDERR_FILENO);
}