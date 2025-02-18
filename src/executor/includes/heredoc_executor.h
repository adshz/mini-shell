/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_executor.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HEREDOC_EXECUTOR_H
# define HEREDOC_EXECUTOR_H

# include "parser/parser.h"

int		setup_heredoc_pipe(t_ast_node *node);
int		collect_heredoc_content(t_ast_node *node, t_shell *shell);
int		handle_heredoc_line(char *line, int pipe_fds[2], \
						t_ast_node *node, t_shell *shell);
int		setup_heredoc(t_ast_node *node, int pipe_fds[2], t_shell *shell);
int		write_heredoc_line(char *line, int pipe_fds[2], t_ast_node *node);

int		handle_redirection_type(t_ast_node *current, t_shell *shell, \
							int saved_stdin, int saved_stdout);
int		setup_saved_fds(int *saved_stdin, int *saved_stdout);
int		handle_output_redirection(t_ast_node *node, t_shell *shell);
int		handle_append_redirection(t_ast_node *node, t_shell *shell);
int		handle_input_redirection(t_ast_node *node, t_shell *shell);
int		handle_heredoc(t_ast_node *node, t_shell *shell);
#endif 
