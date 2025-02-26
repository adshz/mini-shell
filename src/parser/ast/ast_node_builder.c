/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_node_builder.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:27:22 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:28:25 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* AST Node Creation
 *
 *  This file focuses on creating and configuring AST nodes
 *
 */
// Create a new AST node
t_ast_node *ft_create_new_node(t_ast_node_type type);

// Convert token type to AST node type
t_ast_node_type ft_get_ast_node_type(t_token_type type);
