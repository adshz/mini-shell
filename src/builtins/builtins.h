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
# include <types.h>
# include "parser/parser.h"
# include "builtins/ft_echo/echo.h"

/* Builtin command declarations */
int		builtin_cd(t_shell *shell, t_ast_node *node);
int		builtin_echo(t_shell *shell, t_ast_node *node);
int		builtin_env(t_shell *shell, t_ast_node *node);
int		builtin_exit(t_shell *shell, t_ast_node *node);
int		builtin_export(t_shell *shell, t_ast_node *node);
int		builtin_pwd(t_shell *shell, t_ast_node *node);
int		builtin_unset(t_shell *shell, t_ast_node *node);

/* Builtin command utilities */
bool	is_builtin(const char *cmd);
int		execute_builtin(t_shell *shell, t_ast_node *node);

/* CD utilities */
int		handle_cd_path(t_shell *shell, const char *path);
char	*get_oldpwd_path(t_shell *shell);
char	*get_home_path(t_shell *shell);

/* Echo utilities */
bool	is_echo_flag(const char *arg);
void	handle_echo_args(char **args, bool *print_newline);

/* Export utilities */
int		handle_export_arg(t_shell *shell, const char *arg);
bool	is_valid_identifier(const char *str);

#endif
