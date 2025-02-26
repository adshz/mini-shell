/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:37:46 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Creates a command node from a token
 * @param token Token containing command value
 * @return Command node or NULL on failure
 */
t_ast_node	*create_command_from_token(t_token *token)
{
	t_ast_node	*cmd_node;

	cmd_node = create_ast_node(AST_COMMAND, token->value);
	if (!cmd_node)
		return (NULL);
	cmd_node->args = malloc(sizeof(char *) * 2);
	if (!cmd_node->args)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	cmd_node->args[0] = ft_strdup(token->value);
	cmd_node->args[1] = NULL;
	return (cmd_node);
}

/**
 * @brief Attaches a command node to the result tree
 * @param result Current result tree
 * @param cmd_node Command node to attach
 * @return Updated result tree
 */
t_ast_node	*attach_command_node(t_ast_node *result, \
									t_ast_node *cmd_node)
{
	t_ast_node	*current_node;

	if (!result)
		return (cmd_node);
	current_node = result;
	while (current_node->left)
		current_node = current_node->left;
	current_node->left = cmd_node;
	return (result);
}

int	count_command_args(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

t_ast_node	*create_command_from_word(t_token *word_token)
{
	t_ast_node	*node;

	node = init_command_node(word_token, 1);
	if (!node)
		return (NULL);
	if (!fill_args(node->args, word_token, 1))
	{
		free_ast(node);
		return (NULL);
	}
	return (node);
}

t_ast_node	*create_regular_command(t_token **tokens,
		t_token *start, int arg_count)
{
	t_ast_node	*node;
	int			min_args;

	min_args = arg_count;
	if (start && start->value && \
		(ft_strcmp(start->value, "exit") == 0))
	{
		if (arg_count < 3)
			min_args = 3;
		else
			min_args = arg_count;
	}
	node = init_command_node(start, min_args);
	if (!node)
		return (NULL);
	if (!fill_args(node->args, start, arg_count))
	{
		free_ast(node);
		return (NULL);
	}
	skip_processed_args(tokens, arg_count);
	return (node);
}
