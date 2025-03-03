/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:34:37 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:50:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <signal.h>
# include <unistd.h>
# include <termios.h>
# include <stdbool.h>
# include "types.h"
# include "includes/ast_executor.h"
# include "includes/command_executor.h"
# include "includes/path_resolver.h"
# include "includes/pipe_executor.h"
# include "includes/executor_redirection.h"
# include "includes/executor_heredoc.h"
# include "includes/executor_types.h"
# include "includes/executor_utils.h"
# include "includes/builtin_executor.h"
# include "hashtable/hashtable.h"
# include "signals/ft_signals.h"
# include "expander/expander.h"
# include "builtins/builtins.h"
# include "utils/utils.h"
# include "errors.h"
# include <sys/wait.h>
# define WRITE_END 1
# define READ_END 0
# define HEREDOC_CTRL_C 130
# define NORMAL_OPERATION 0
# define EXIT_NORMAL 0
# define HEREDOC_CTRL_D 0

extern volatile sig_atomic_t g_signal_status;

/* Main execution function entry*/
void	execute_shell_command(t_shell *shell);
#endif
