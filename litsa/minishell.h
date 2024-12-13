#ifndef MINISHELL_H
# define MINISHELL_H

#include "./libft/libft.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <sys/_types/_pid_t.h>
#include <fcntl.h>

void	echo(char *input);
void    pwd(void);
void    cd(char *input);
void	env(void);
void	export(char *input);
void	show_var(char *inut);
void	unset(char *input);
void	redirect_output(char *input);
void	execute_command(char *input);

#endif