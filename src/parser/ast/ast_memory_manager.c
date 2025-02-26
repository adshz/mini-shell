/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:28:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:29:30 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* AST memory handling
 *
 * This file manages memory for the AST, including cleanup
 */
// Clean up AST nodes recursively
void recursively_ft_cleanup_ast(t_ast_node *node);

// Clean up entire AST and related resources
void ft_cleanup_ast(t_shell *shell, t_ast_node **ast);

// Clean up a command node's resources
void clean_cmd_node(t_ast_node *node);

// Free 2D char array -> this can go to `utils` module since it is a common one
void free_char2(char **tofree);

// Clean up token list
void clean_token_lst(t_token **lst);


