/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 echo.h												:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/16 21:23:32 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/16 21:23:32 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#ifndef ECHO_H
# define ECHO_H

# include <stdlib.h>
# include "shell.h"
# include "libft.h"

/* Expansion state structure */
typedef struct s_expansion_state
{
	int	i;
	int	j;
	int	in_single_quotes;
	int	in_double_quotes;
	int	is_command_name;
	int	was_quoted_var;
	int	prev_was_space;
	int	had_var_expansion;
}	t_expansion_state;

/* Main echo function */
int		ft_echo(t_shell *shell, t_ast_node *node);

/* Echo utilities */
bool	is_echo_flag(const char *arg);
void	handle_echo_args(char **args, bool *print_newline);

/* Variable expansion functions */
void	init_expansion_state(t_expansion_state *state, const char *arg);
void	handle_quotes(char quote, t_expansion_state *state);
void	handle_exit_status(t_shell *shell, char *result, \
					t_expansion_state *state);
void	handle_variable(t_shell *shell, const char *arg, char *result, \
					t_expansion_state *state);
void	handle_regular_char(char c, char *result, t_expansion_state *state);
char	*finalize_result(char *result, t_expansion_state *state);

/* Command handling functions */
void	copy_command_word(char *result, const char *value, \
					t_expansion_state *state);
void	copy_command_args(char *result, const char *value, \
					t_expansion_state *state);
void	copy_variable_value(char *result, const char *value, \
						t_expansion_state *state);

/* Utility functions */
void	ft_itoa_buf(int n, char *buf);

#endif 
