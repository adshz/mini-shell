/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_handler.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_NODE_HANDLER_H
# define AST_NODE_HANDLER_H

# include "parser_types.h"

/* AST node functions */
void        free_ast(t_ast_node *ast);
t_ast_node  *create_ast_node(t_ast_type type, char *value);
bool        is_valid_ast(void *ast);

/* Command node functions */
t_ast_node  *create_command_from_word(t_token *word_token);
t_ast_node  *create_regular_command(t_token **tokens, t_token *start,
                                int arg_count);
t_ast_node  *create_command_from_token(t_token *token);
t_ast_node  *attach_command_node(t_ast_node *result, t_ast_node *cmd_node);

/* Command initialization helpers */
char        **allocate_args(int arg_count);
int         is_variable_token(const char *value);
bool        init_command_expansion(t_ast_node *node, const char *value);
int         fill_args(char **args, t_token *start, int arg_count);
void        skip_processed_args(t_token **tokens, int count);

/* Command node utilities */
t_token     *find_last_word_token(t_token *start);
int         count_command_args(t_token *tokens);
int         count_args(t_token *start);
t_ast_node  *init_command_node(t_token *start, int arg_count);

#endif 