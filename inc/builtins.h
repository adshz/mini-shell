/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:32 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:50:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "shell.h"
# include "parser.h"

/* Builtin command functions */
int		builtin_echo(t_shell *shell, t_ast_node *node);
int		builtin_cd(t_shell *shell, t_ast_node *node);
int		builtin_pwd(t_shell *shell, t_ast_node *node);
int		builtin_export(t_shell *shell, t_ast_node *node);
int		builtin_unset(t_shell *shell, t_ast_node *node);
int		builtin_env(t_shell *shell, t_ast_node *node);
int		builtin_exit(t_shell *shell, t_ast_node *node);

/* Builtin utilities */
bool	is_builtin(const char *cmd);
int		execute_builtin(t_shell *shell, t_ast_node *node);
char	*find_command(t_shell *shell, const char *cmd);

// PWD builtin
char    *ft_pwd(t_shell *shell);
int     builtin_pwd(t_shell *shell, t_ast_node *node);

#endif


