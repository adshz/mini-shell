/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:04:14 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:06:46 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANDER_H
# define EXPANDER_H

# include "shell.h"
# include <stdlib.h>
# include <unistd.h>
# include "libft.h"

typedef struct s_copy_state
{
	char		*result;
	const char	*str;
	size_t		i;
	size_t		j;
}	t_copy_state;

typedef struct s_cmd_expand
{
	char	*expanded_value;
	char	**cmd_args;
	char	*prefix;
	char	*dollar_pos;
	char	*full_cmd;
	size_t	prefix_len;
}	t_cmd_expand;

/* Forward declarations */
struct						s_shell;
typedef struct s_shell		t_shell;

char	**expand_command(t_shell *shell, const char *cmd);
char	*expand_simple_variable(t_shell *shell, const char *arg);
char	*expand_variable(t_shell *shell, const char *var_name);
char	*expand_tilde(t_shell *shell, const char *arg);

void	init_cmd_expand(t_cmd_expand *exp);
char	**handle_variable_command(t_shell *shell, const char *cmd);

void	free_expanded_args(char **args);
char	*get_variable_name(const char *str, size_t *i);
int		is_valid_variable_char(char c);
char	*strip_quotes(char *str);
char	*handle_quoted_string(const char *str, size_t *i, char quote_char);
int		is_quoted(const char *str);
char	*expand_exit_status(t_shell *shell);
char	*expand_home_dir(t_shell *shell, const char *arg);

#endif