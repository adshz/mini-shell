/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_handler.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPELINE_HANDLER_H
# define PIPELINE_HANDLER_H

# include "parser_types.h"

t_ast_node	*parse_pipeline(t_token **tokens, t_shell *shell);
t_ast_node	*process_pipeline(t_token **tokens);
t_ast_node	*handle_pipeline_sequence(t_token **tokens);
bool		is_pipeline_token(t_token *token);
bool		validate_pipeline_sequence(t_token *tokens);
bool		is_valid_pipeline_component(t_ast_node *node);
t_token		*find_next_pipe(t_token *token);
t_token		*skip_pipeline_sequence(t_token *token);
void		cleanup_pipeline_node(t_ast_node *node);
t_ast_node	*parse_pipeline_component(t_token **tokens);
int			count_pipeline_components(t_token *tokens);
#endif
