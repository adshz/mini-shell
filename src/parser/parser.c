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
 * @brief Handles expansion of variables in a token value
 * 
 * Processes a token value that contains variables ($ syntax):
 * - For pure variable tokens ($VAR)
 * - For mixed tokens (prefix$VAR)
 * 
 * @param shell Shell context for variable lookup
 * @param value Token value to process
 * @return Expanded string or NULL on error
 */
static char	*parse_handle_variable_expansion(t_shell *shell, const char *value)
{
	char	*expanded_value;
	char	*dollar_pos;
	char	*prefix;
	char	*final_value;
	char	*remaining_expanded;

	if (!value)
		return (NULL);
	if (value[0] == '$')
	{
		expanded_value = expand_simple_variable(shell, value + 1);
		if (!expanded_value)
			expanded_value = ft_strdup("");
		dollar_pos = ft_strchr(value + 1, '$');
		if (dollar_pos)
		{
			remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
			if (remaining_expanded)
			{
				final_value = ft_strjoin(expanded_value, remaining_expanded);
				free(expanded_value);
				free(remaining_expanded);
				expanded_value = final_value;
			}
		}
		return (expanded_value);
	}
	dollar_pos = ft_strchr(value, '$');
	if (!dollar_pos)
		return (ft_strdup(value));
	prefix = ft_substr(value, 0, dollar_pos - value);
	if (!prefix)
		return (NULL);
	remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
	if (!remaining_expanded)
	{
		free(prefix);
		return (NULL);
	}
	final_value = ft_strjoin(prefix, remaining_expanded);
	free(prefix);
	free(remaining_expanded);
	return (final_value);
}

/**
 * @brief Parses an expression from the token stream.
 *
 * An expression can be:
 * - Simple command: "echo hello"
 * - Pipeline: "ls | grep foo"
 * - Command with redirections: "echo hello > file"
 *
 * Process:
 * 1. Parse pipeline structure
 * 2. Handle any trailing redirections
 *
 * @param tokens Pointer to current position in token stream
 * @return AST node representing the expression, NULL on error
 *
 * @note Advances token pointer during parsing
 * @see parse_pipeline() for pipeline handling
 * @see parse_redirection_construct() for redirection handling
 */
t_ast_node	*parse_expression(t_token **tokens, t_shell *shell)
{
	t_ast_node	*node;
	char		*expanded;

	if (!tokens || !*tokens)
		return (NULL);
	if (ft_strchr((*tokens)->value, '$'))
	{
		char *dollar_pos = ft_strchr((*tokens)->value, '$');
		if (dollar_pos > (*tokens)->value)
		{
			if ((*tokens)->next && ft_strchr((*tokens)->next->value, '$'))
			{
				expanded = parse_handle_variable_expansion(shell, (*tokens)->next->value);
				if (!expanded)
					return (NULL);
				free((*tokens)->next->value);
				(*tokens)->next->value = expanded;
			}
		}
		else
		{
			expanded = parse_handle_variable_expansion(shell, (*tokens)->value);
			if (!expanded)
				return (NULL);
			free((*tokens)->value);
			(*tokens)->value = expanded;
		}
	}
	node = parse_pipeline(tokens, shell);
	if (!node)
		return (NULL);
	if (*tokens && (*tokens)->type == TOKEN_REDIRECT_IN)
		node = parse_redirection_construct(node, tokens, shell);
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

	if (!tokens)
		return (NULL);
	if (tokens->type == TOKEN_PIPE)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", STDERR_FILENO);
		shell->exit_status = 258;
		return (NULL);
	}
	current = tokens;
	ast = parse_expression(&current, shell);
	if (!ast)
	{
		if (current && is_redirection_token(current->type))
			shell->exit_status = 258;
		return (NULL);
	}
	if (current != NULL)
	{
		free_ast(ast);
		shell->exit_status = 258;
		return (NULL);
	}
	return (ast);
}