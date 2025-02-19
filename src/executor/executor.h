/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:34:37 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:48:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include <signal.h>
// Core includes
# include "includes/heredoc_executor.h"
# include "includes/command_executor.h"
# include "includes/pipe_executor.h"
# include "includes/env_executor.h"
# include "includes/command_expander.h"
# include "includes/signal_handler.h"
# include "includes/redirections_executor.h"
# include "includes/command_check.h"
# include "utils/utils.h"
# include "errors.h"
# include <sys/wait.h>

/* High-level orchestration functions */
void	restore_redirections(t_shell *shell);
void	add_command_history(t_shell *shell, void *ast);
int		handle_node_by_type(t_shell *shell, t_ast_node *node);

/* Builtin redirection handling */
int		handle_cd_redirection(t_shell *shell, t_ast_node *node);
int		handle_other_builtin_redirection(t_shell *shell, t_ast_node *node);
int		handle_builtin_redirection(t_shell *shell, t_ast_node *node);
int		handle_redirection_node(t_shell *shell, t_ast_node *node);

/* Path resolution */
int		handle_path_resolution(t_shell *shell, t_ast_node *node, \
						char **cmd_path);

#endif
