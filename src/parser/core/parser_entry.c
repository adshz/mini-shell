/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_entry.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:21:04 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:21:54 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

// Main entry point for parsing tokens into an AST
t_ast_node *parse(t_shell *shell, t_token *tokens_lst);

// Function to build AST tree from tokenized input
static bool build_ast_tree(t_shell *shell);

// High-level function to parse input and build the AST
bool parse_and_build_ast(t_shell *shell);


