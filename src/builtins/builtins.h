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
# include "builtins/ft_export/export.h"
# include "builtins/ft_cd/cd.h"

/* Builtin command declarations */
int		builtin_cd(t_shell *shell, t_ast_node *node);
int		ft_echo(t_shell *shell, t_ast_node *node);
int		builtin_env(t_shell *shell, t_ast_node *node);
int		builtin_exit(t_shell *shell, t_ast_node *node);
int		builtin_export(t_shell *shell, t_ast_node *node);
int		builtin_pwd(t_shell *shell, t_ast_node *node);
int		builtin_unset(t_shell *shell, t_ast_node *node);

/* General builtin utilities */
bool	is_builtin(const char *cmd);
int		execute_builtin(t_shell *shell, t_ast_node *node);

#endif
