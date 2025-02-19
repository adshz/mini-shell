/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:43:46 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 18:44:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_PARSER_H
# define COMMAND_PARSER_H

t_ast_node	*parse_command(t_token **tokens, t_shell *shell);
t_ast_node	*init_command_node(t_token *start, int arg_count);
t_ast_node	*create_default_heredoc_command(void);
int			fill_args(char **args, t_token *start, int arg_count);
char		**allocate_args(int arg_count);
bool		init_command_expansion(t_ast_node *node, const char *value);
int			count_command_args(t_token *tokens);
t_ast_node	*create_command_from_word(t_token *word_token);
t_ast_node	*create_regular_command(t_token **tokens, \
								t_token *start, int arg_count);
t_ast_node	*create_command_from_token(t_token *token);
t_ast_node	*attach_command_node(t_ast_node *result, t_ast_node *cmd_node);
int			count_args(t_token *tokens);
void		skip_processed_args(t_token **tokens, int count);
int			is_variable_token(const char *token);

#endif
