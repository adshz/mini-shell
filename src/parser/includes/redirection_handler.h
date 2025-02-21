/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:42:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 18:43:27 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_HANDLER_H
# define REDIRECTION_HANDLER_H

# include "parser_types.h"

t_ast_node	*create_redir_file_node(const char *file_value, \
								t_ast_node *redir_node);
t_ast_node	*parse_redirection_construct(t_ast_node *left, t_token **tokens, \
									t_shell *shell);
int			is_redirection_token(t_token_type type);
t_ast_node	*create_redirection_node(t_token_type type, \
										const char *file_value);
t_ast_node	*create_default_heredoc_command(void);
t_ast_node	*create_heredoc_chain(t_token **tokens, \
								t_ast_node *command_node, t_shell *shell);
t_ast_node	*cleanup_heredoc_nodes(t_ast_node *first_redir, \
										t_ast_node *command_node);
t_ast_node	*process_redir_token(t_redir_parse *parse, t_token **tokens);
t_ast_node	*handle_heredoc_token(t_redir_token *rt);
t_ast_node	*create_and_attach_command(t_word_token *wt);
t_ast_node	*is_word_part_of_redirection(t_word_token *wt);
t_ast_node	*handle_word_token(t_word_token *wt);
void		init_word_token(t_word_token *wt, t_redir_parse *parse, \
					t_token **tokens);
void		update_token_positions(t_word_token *wt);

#endif
