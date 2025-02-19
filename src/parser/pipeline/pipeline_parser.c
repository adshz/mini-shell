/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:03:52 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 12:05:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Creates a new pipeline node connecting two AST nodes
 *
 * Creates a pipe node (|) with left and right subtrees:
 * Example: "ls | grep foo" becomes:
 *        PIPE
 *        /  \
 *       ls  grep
 *           /
 *         foo
 *
 * @param left  Left side of pipe (command before |)
 * @param right Right side of pipe (command after |)
 * @return New pipe node, NULL on allocation failure
 *
 * @note Handles memory cleanup on failure
 * @see create_ast_node() for node allocation
 */
static t_ast_node	*create_pipeline_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*pipe_node;

	pipe_node = create_ast_node(AST_PIPE, NULL);
	if (!pipe_node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

/**
 * @brief Parses the left side of a pipeline
 *
 * Processes command and any redirections before pipe:
 * Example: "cat file > temp |" -> parses "cat file > temp"
 *
 * @param tokens Current position in token stream
 * @param shell Shell context for error handling
 * @return AST node for left side of pipeline, NULL on error
 *
 * @see parse_command() for command parsing
 * @see process_consecutive_redirections() for redirection handling
 */
static t_ast_node	*parse_pipeline_left(t_token **tokens, t_shell *shell)
{
	t_ast_node	*left;

	left = parse_command(tokens, shell);
	if (!left)
		return (NULL);
	left = process_consecutive_redirections(left, tokens, shell);
	if (!left)
		return (NULL);
	return (left);
}

/**
 * @brief Parses the right side of a pipeline
 *
 * Handles:
 * 1. Pipeline token validation
 * 2. Recursive pipeline parsing for multiple pipes
 * 3. Error reporting for dangling pipes
 *
 * Example: "| grep foo | wc -l" -> parses "grep foo | wc -l"
 *
 * @param tokens Current position in token stream
 * @param left   AST node for left side of pipeline
 * @param shell  Shell context for error handling
 * @return Complete pipeline AST, NULL on error
 *
 * @note Reports syntax error for dangling pipes
 * @see parse_pipeline() for recursive parsing
 */
static t_ast_node	*parse_pipeline_right(t_token **tokens, t_ast_node *left, \
										t_shell *shell)
{
	t_ast_node	*right;
	t_token		*current;

	current = *tokens;
	if (!current || current->type != TOKEN_PIPE)
		return (left);
	*tokens = current->next;
	if (!tokens)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token '|'", \
				STDERR_FILENO);
		shell->exit_status = 2;
		free_ast(left);
		return (NULL);
	}
	right = parse_pipeline(tokens, shell);
	if (!right)
	{
		free_ast(left);
		return (NULL);
	}
	return (create_pipeline_node(left, right));
}

/**
 * @brief Main pipeline parsing function
 *
 * Parses complete pipeline constructs like:
 * - Simple command: "ls -l"
 * - Single pipe: "ls | grep foo"
 * - Multiple pipes: "ls | grep foo | wc -l"
 *
 * Process:
 * 1. Parse left side (command)
 * 2. If pipe token found, parse right side
 * 3. Build pipeline AST structure
 *
 * @param tokens Current position in token stream
 * @param shell Shell context for error handling
 * @return AST representing complete pipeline, NULL on error
 *
 * @note Handles memory cleanup on partial parse failure
 * @see parse_pipeline_left() for left side parsing
 * @see parse_pipeline_right() for right side parsing
 */
t_ast_node	*parse_pipeline(t_token **tokens, t_shell *shell)
{
	t_ast_node	*left;

	if (!tokens || !*tokens)
		return (NULL);
	left = parse_pipeline_left(tokens, shell);
	if (!left)
		return (NULL);
	return (parse_pipeline_right(tokens, left, shell));
}
