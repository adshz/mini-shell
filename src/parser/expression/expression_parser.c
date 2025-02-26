/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expression.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 07:59:26 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 07:59:51 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

// Main expression parsing function
t_ast_node *ft_parse_expression(t_shell *shell, int min_prec);

// Get precedence level for a token type
int ft_precedence(t_token_type type);

// Get precedence of current token
int ft_curr_token_precd(t_shell *shell);

// Check if current token is a binary operator
bool is_binary_operator(t_shell *shell);

// Combine left and right nodes with an operator
t_ast_node *ft_combine_node(t_shell *shell, t_token_type operator, t_ast_node *left, t_ast_node *right);

