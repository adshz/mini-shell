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
#include "parser.h"

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
		ft_putstr_fd("Failed to initialize command expansion\n", STDERR_FILENO);
		free_ast(node);
		return (NULL);
	}
	node->args = allocate_args(arg_count);
	if (!node->args)
	{
		ft_putstr_fd("Failed to allocate arguments\n", STDERR_FILENO);
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

/**
 * @brief Fills argument array with duplicated token values
 * @param args Array to fill with arguments
 * @param start Starting token
 * @param arg_count Number of arguments to process
 * @return 1 on success, 0 on failure
 * 
 * Duplicates token values into the args array. Handles both
 * regular tokens and variable tokens. Ensures proper memory
 * allocation for each argument.
 */
int	fill_args(char **args, t_token *start, int arg_count)
{
	t_token	*current;
	int		  i;
	int     allocated_size;

	current = start;
	i = 0;
	allocated_size = arg_count;
	if (start && start->value && ft_strcmp(start->value, "exit") == 0)
	{
		if (arg_count < 3)
			allocated_size = 3;
		else
			allocated_size = arg_count;
	}
	while (i < arg_count && current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
			return (0);
		current = current->next;
		i++;
	}
	while (i < allocated_size)
	{
		args[i] = NULL;
		i++;
	}
	args[allocated_size] = NULL;
	return (1);
}

t_ast_node	*create_default_heredoc_command(void)
{
	t_token		*cat_token;
	t_ast_node	*node;

	cat_token = create_token(TOKEN_WORD, "cat");
	if (!cat_token)
	{
		ft_putstr_fd("Failed to create cat token\n", STDERR_FILENO);
		return (NULL);
	}
	node = init_command_node(cat_token, 1);
	if (!node)
	{
		ft_putstr_fd("Failed to initialize command node\n", STDERR_FILENO);
		free_tokens(cat_token);
		return (NULL);
	}
	if (!fill_args(node->args, cat_token, 1))
	{
		ft_putstr_fd("Failed to fill arguments\n", STDERR_FILENO);
		free_tokens(cat_token);
		free_ast(node);
		return (NULL);
	}
	free_tokens(cat_token);
	return (node);
}
