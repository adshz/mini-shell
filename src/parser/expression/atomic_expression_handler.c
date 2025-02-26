/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   atomic_expression_handler.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:25:21 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:27:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Terminal Expression Parsing
 *
 * This file handles parsing of atomic expressions such as (commands and
 * parenthesizsed expression
 *
 */
// Parse atomic expressions (terminals in the grammar)
t_ast_node *ft_parse_atomic_expression(t_shell *shell);



