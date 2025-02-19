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

/* Command node utilities */
t_token     *find_last_word_token(t_token *start);

#endif 