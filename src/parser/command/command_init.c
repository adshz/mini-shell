/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:38:36 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 13:38:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Initializes command node with variable expansion
 * @param node Node to initialize
 * @param value Value to check for expansion
 * @return true on success, false on failure
 */
bool	init_command_expansion(t_ast_node *node, const char *value)
{
	node->is_expanded = is_variable_token(value);
	if (node->is_expanded)
	{
		node->original = ft_strdup(value);
		if (!node->original)
			return (false);
	}
	return (true);
}

/**
 * @brief Creates initial command node structure
 * @param start Starting token
 * @param arg_count Number of arguments
 * @return Command node or NULL on failure
 */
t_ast_node	*init_command_node(t_token *start, int arg_count)
{
	t_ast_node	*node;

	node = create_ast_node(AST_COMMAND, start->value);
	if (!node)
		return (NULL);
	if (!init_command_expansion(node, start->value))
	{
		free_ast(node);
		return (NULL);
	}
	node->args = allocate_args(arg_count);
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

/**
 * @brief Allocates memory for argument array
 * @param arg_count Number of arguments to allocate
 * @return Allocated array or NULL on failure
 * 
 * Allocates memory for arg_count + 1 pointers, with the last
 * pointer set to NULL as a terminator.
 */
char	**allocate_args(int arg_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	return (args);
}

t_ast_node	*create_default_heredoc_command(void)
{
	t_token		*cat_token;
	t_ast_node	*node;

	cat_token = create_token(TOKEN_WORD, "cat");
	if (!cat_token)
		return (NULL);
	node = init_command_node(cat_token, 1);
	if (!node)
	{
		free_tokens(cat_token);
		return (NULL);
	}
	if (!fill_args(node->args, cat_token, 1))
	{
		free_tokens(cat_token);
		free_ast(node);
		return (NULL);
	}
	free_tokens(cat_token);
	if (g_signal_status == SIG_HEREDOC_INT)
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}
