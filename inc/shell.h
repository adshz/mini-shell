/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:44:07 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:03 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHELL_H
# define SHELL_H
# include "libft.h"
// delete it later
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "libft.h"
# include <signal.h>
# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <limits.h>
# include <termios.h>
# include <stdbool.h>
# include "types.h"
# include "lexer.h"
# include "parser.h"
# include "hashtable.h"
# include "signals.h"
# include "errors.h"
# include "utils.h"

# define PROMPT ">>minishell$ "
/* Configure Return values for readability */
# define SUCCESS 0
# define ERROR 1
# define SYNTAX_ERROR 2

/* Global signal variable */
extern int	g_signal;

/* Core functions */
void	init_shell(t_shell *shell, char *argv[], char *envp[]);
void	cleanup_shell(t_shell *shell);
void	free_history(char **history);

#endif
