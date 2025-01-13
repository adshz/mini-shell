/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:25 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:50:25 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
#define EXECUTOR_H

#include "shell.h"
#include "parser.h"
#include "types.h"
#include "libft.h"

// Exit status codes
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#define EXIT_CMD_NOT_FOUND 127

// Function declarations for AST execution
int execute_ast(t_shell *shell, t_ast_node *ast);
int execute_command(t_shell *shell, t_ast_node *node);
int execute_pipe(t_shell *shell, t_ast_node *node);
int execute_redirection(t_shell *shell, t_ast_node *node);

// Function declarations for redirection handling
int execute_redirections(t_ast_node *node, t_shell *shell);
int handle_input_redirection(t_ast_node *node, t_shell *shell);
int handle_output_redirection(t_ast_node *node, t_shell *shell);
int handle_append_redirection(t_ast_node *node, t_shell *shell);
int handle_heredoc(t_ast_node *node);
int is_ambiguous_redirect(t_shell *shell, const char *value);

// Helper functions
void setup_redirections(t_shell *shell, t_ast_node *node);
char *get_expanded_filename(t_shell *shell, const char *filename);

#endif


