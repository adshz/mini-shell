/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/07 10:00:00 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

int count_command_args(t_token *tokens)
{
	int count;

	count = 0;
	while (tokens && tokens->type == TOKEN_WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

t_ast_node *create_command_from_word(t_token *word_token)
{
	t_ast_node *node;

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

t_ast_node *create_regular_command(t_token **tokens, t_token *start, int arg_count)
{
	t_ast_node *node;

	node = init_command_node(start, arg_count);
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