/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:44:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:46:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CORE_H
# define CORE_H
// Main entry point for parsing tokens into an AST
t_ast_node	*parse(t_shell *shell, t_token *tokens_lst);

// High-level function to parse input and build the AST
bool		parse_and_build_ast(t_shell *shell);

// Function to build AST tree from tokenized input
static bool	build_ast_tree(t_shell *shell);
#endif
