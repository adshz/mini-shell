/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:47:52 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:15:40 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer/lexer.h"
#include "expander/expander.h"

static bool	validate_initial_pipe(t_token *token, t_shell *shell)
{
	if (token->type == TOKEN_PIPE)
	{
		ft_putendl_fd(\
				"minishell: syntax error near unexpected token '|'", \
				STDERR_FILENO);
		shell->exit_status = 258;
		return (false);
	}
	return (true);
}

static t_ast_node	*is_parse_error(t_token *current, t_shell *shell)
{
	if (current && is_redirection_token(current->type))
		shell->exit_status = 258;
	return (NULL);
}

static bool	validate_remaining_tokens(t_token *current, t_ast_node *ast, \
									t_shell *shell)
{
	if (current != NULL)
	{
		free_ast(ast);
		shell->exit_status = 258;
		return (false);
	}
	return (true);
}

/**
 * @brief Processes multiple consecutive redirection tokens
 * @param node Current AST node (usually a command node)
 * @param tokens Token stream
 * @param shell Shell context for error handling
 * @return Updated AST node with all redirections processed, or NULL on failure
 * 
 * Examples:
 * "echo hello > file1 > file2" becomes:
 *             >
 *            / \
 *           >   file2
 *          / \
 *         /   file1
 *        /
 *     echo
 *      |
 *    hello
 *
 * Where:
 * - echo is a command node with "hello" as its argument
 * - Each '>' is a redirection node
 * - file1 and file2 are the redirection targets
 */
t_ast_node	*process_consecutive_redirections(t_ast_node *node,
											t_token **tokens,
											t_shell *shell)
{
	t_token	*current;

	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		node = parse_redirection_construct(node, tokens, shell);
		if (!node)
			return (NULL);
		current = *tokens;
	}
	return (node);
}

/**
 * @brief Main parsing function for shell command input
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
t_ast_node	*parse(t_token *tokens, t_shell *shell)
{
	t_ast_node	*ast;
	t_token		*current;

	ast = NULL;
	if (!tokens)
		return (NULL);
	if (!validate_initial_pipe(tokens, shell))
		return (NULL);
	current = tokens;
	ast = parse_expression(&current, shell);
	if (!ast)
		return (is_parse_error(current, shell));
	if (!validate_remaining_tokens(current, ast, shell))
		return (NULL);
	return (ast);
}
