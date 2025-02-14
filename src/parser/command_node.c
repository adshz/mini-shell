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

#include "parser.h"

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
	int         min_args;

	// Ensure we allocate at least 3 slots for builtins that might need them
	min_args = arg_count;
	if (start && start->value && (ft_strcmp(start->value, "exit") == 0))
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
