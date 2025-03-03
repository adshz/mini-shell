/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:59:53 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 23:16:50 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdbool.h>
# include <stdlib.h>
# include <limits.h>
# include "libft.h"
# include "types.h"
# include "parser/parser.h"
# include "hashtable/hashtable.h"
# include "executor/executor.h"
# include "utils/utils.h"


/* Builtin command declarations */
int	builtin_cd(t_shell *shell, char *path);
int	builtin_echo(char **argv);
int	builtin_env(t_shell *shell);
void	builtin_exit(t_shell *shell, char **argv);
int	builtin_export(t_shell *shell, char **argv);
int	builtin_pwd(void);
int	builtin_unset(t_shell *shell, char **argv);
bool	check_valid_key(char *key);

#endif
