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
# define EXECUTOR_H

# include "types.h"
# include "parser.h"
# include "utils.h"
# include "hashtable.h"
# include <unistd.h>

// Exit status codes
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
# define EXIT_CMD_NOT_FOUND 127

// Function prototypes
int     execute_ast(t_shell *shell, t_ast_node *ast);
int     execute_command(t_ast_node *node, t_hashmap *env);
int     execute_pipe(t_shell *shell, t_ast_node *node);
int     execute_redirection(t_shell *shell, t_ast_node *node);
void    handle_redirections(t_shell *shell, t_ast_node *node);
pid_t   create_process(t_shell *shell);

// Pipe handling
void    setup_pipes(int old_pipe[2], int new_pipe[2], int is_first, int is_last);
void    close_pipes(int old_pipe[2], int new_pipe[2]);

// Heredoc handling
char    *process_heredoc(t_shell *shell, const char *delimiter);
void    cleanup_heredoc(char *heredoc_content);

// Command execution helpers
char    *get_command_path(const char *cmd, t_hashmap *env);
int     handle_pipe(t_shell *shell, t_ast_node *node);

#endif


