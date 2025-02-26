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


/**
 * @brief Main parsing function for shell tokenised input
 *
 * Constructs Abstract Syntax Tree (AST) from token stream.
 * Handles syntax validation and error reporting.
 *
 * Process:
 * 1. Validate initial token (can't start with pipe)
 * 2. Parse complete expression
 * 3. Ensure all tokens are consumed
 *
 * @param tokens Head of token list to parse
 * @return Root of AST, NULL on syntax error
 *
 * Examples:
 * - "echo hello" -> COMMAND("echo", ["hello"])
 * - "ls | grep foo" -> PIPE(COMMAND("ls"), COMMAND("grep", ["foo"]))
 * - "echo hello > file" -> REDIRECT(COMMAND("echo", ["hello"]), "file")
 *
 * @note Frees partial AST on syntax error
 * @see parse_expression() for expression parsing
 */
t_ast_node	*parse(t_shell *shell, t_token *tokens_lst)
{
	t_ast_node	*ast;

	ast = NULL;
	shell->curr_token = tokens_lst;
	ast = ft_parse_expression(shell, 0);
	if (shell->curr_token)
		return (set_parse_err(E_SYNTAX), ast);
	return (ast);
}
