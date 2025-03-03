/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:56:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 20:55:36 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_SIGNALS_H
# define FT_SIGNALS_H

# define SHELL_STATE_READLINE 1
# define SHELL_STATE_EXECUTION 2
# define SHELL_STATE_HEREDOC_INPUT 42
# define SHELL_STATE_HEREDOC_INTERRUPTED 43
# define SHELL_STATE_NORMAL 0
# include <signal.h>
# include <termios.h>
# include <unistd.h>
# include <errno.h>
# include "libft.h"
# include "types.h"
# include <readline/readline.h>
# include <readline/history.h>

void	config_main_signals(void);
void	config_execution_signals(void);
int	get_signal_from_status(int status);
int	exited_normally(int status);
int	get_exit_status(int status);
int	was_signaled(int status);
#endif
