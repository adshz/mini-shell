/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections_executor.h                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:48:57 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 14:50:31 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef REDIRECTIONS_EXECUTOR_H
# define REDIRECTIONS_EXECUTOR_H

void	setup_redirections(t_shell *shell, t_ast_node *node);
int		execute_redirection(t_shell *shell, t_ast_node *node);
int		execute_child_process(t_shell *shell, t_ast_node *node);
int		handle_redirection_parent_process(t_shell *shell, int status);
char	*expand_redirection_filename_variable(t_shell *shell, \
										const char *filename);
char	*remove_quotes(const char *filename);
char	*handle_quoted_variable(t_shell *shell, char *unquoted);
char	*handle_quoted_filename(t_shell *shell, const char *filename);
int		is_ambiguous_redirect(t_shell *shell, const char *value);
char	*get_expanded_filename(t_shell *shell, const char *filename);
int		handle_ambiguous_filename(const char *filename);
void	cleanup_and_exit(int saved_stdin, int saved_stdout, int status);
int		handle_redirection_type(t_ast_node *current, t_shell *shell, \
							int saved_stdin, int saved_stdout);
int		collect_redirection_nodes(t_ast_node *node, \
							t_ast_node *redir_nodes[], int *redir_count);
#endif
