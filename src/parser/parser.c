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
	char	*remaining_expanded; // added

	ft_putstr_fd("\nDEBUG: parse_handle_variable_expansion called with value: [", STDERR_FILENO);
	ft_putstr_fd(value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	if (!value)
		return (NULL);

	// Handle pure variable case (starts with $)
	if (value[0] == '$')
	{
		ft_putstr_fd("DEBUG: Pure variable expansion (starts with $)\n", STDERR_FILENO);
		expanded_value = expand_simple_variable(shell, value + 1);
		if (!expanded_value)
		{
			ft_putstr_fd("DEBUG: Variable expansion returned NULL, using empty string\n", STDERR_FILENO);
			expanded_value = ft_strdup("");
		}
		
		// Check for more variables after the first expansion
		dollar_pos = ft_strchr(value + 1, '$');
		if (dollar_pos)
		{
			ft_putstr_fd("DEBUG: Found another $ after expansion, recursively expanding\n", STDERR_FILENO);
			remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
			if (remaining_expanded)
			{
				final_value = ft_strjoin(expanded_value, remaining_expanded);
				free(expanded_value);
				free(remaining_expanded);
				expanded_value = final_value;
			}
		}

		ft_putstr_fd("DEBUG: Final expansion result: [", STDERR_FILENO);
		ft_putstr_fd(expanded_value, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		return (expanded_value);
	}

	// Handle mixed case ($ in middle of string)
	dollar_pos = ft_strchr(value, '$');
	if (!dollar_pos)
	{
		ft_putstr_fd("DEBUG: No $ found in value, returning copy\n", STDERR_FILENO);
		return (ft_strdup(value));
	}
	
	ft_putstr_fd("DEBUG: Mixed variable expansion ($ in middle)\n", STDERR_FILENO);
	prefix = ft_substr(value, 0, dollar_pos - value);
	if (!prefix)
	{
		ft_putstr_fd("DEBUG: Failed to extract prefix\n", STDERR_FILENO);
		return (NULL);
	}

	// Recursively expand the rest of the string starting from the $
	remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
	if (!remaining_expanded)
	{
		free(prefix);
		return (NULL);
	}

	final_value = ft_strjoin(prefix, remaining_expanded);
	free(prefix);
	free(remaining_expanded);
	
	if (final_value)
	{
		ft_putstr_fd("DEBUG: Final mixed expansion result: [", STDERR_FILENO);
		ft_putstr_fd(final_value, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("DEBUG: Failed to join prefix and expanded value\n", STDERR_FILENO);
	
	return (final_value);
}

/**
 * @brief Parses a shell expression from token stream
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
	/* debugg */
	ft_putstr_fd("\nDEBUG: parse_expression checking for variables in token: [", STDERR_FILENO);
	ft_putstr_fd((*tokens)->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	/* debugg */
	
	// Only handle pure variable cases (starting with $)
	if ((*tokens)->value[0] == '$')
	{
		ft_putstr_fd("DEBUG: Found pure variable token, expanding...\n", STDERR_FILENO);
		expanded = parse_handle_variable_expansion(shell, (*tokens)->value);
		if (!expanded)
		{
			ft_putstr_fd("DEBUG: Variable expansion failed\n", STDERR_FILENO);
			return (NULL);
		}
		ft_putstr_fd("DEBUG: Expanded value: [", STDERR_FILENO);
		ft_putstr_fd(expanded, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		
		// Update the token's value instead of creating a new node
		free((*tokens)->value);
		(*tokens)->value = expanded;  // Transfer ownership of expanded string
		ft_putstr_fd("DEBUG: Updated token value, continuing with pipeline parsing\n", STDERR_FILENO);
	}
	/* debugg */
	ft_putstr_fd("DEBUG: Proceeding with pipeline parsing\n", STDERR_FILENO);
	/* debugg */
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
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", \
				STDERR_FILENO);
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
