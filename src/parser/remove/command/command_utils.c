/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:38:48 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:39:26 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"

/**
 * @brief Counts the number of consecutive word tokens
 * @param tokens Starting token of the token stream
 * @return Number of consecutive word tokens
 * 
 * Counts consecutive tokens of type TOKEN_WORD until a different
 * token type is encountered or the token stream ends.
 */
int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Advances token pointer past processed arguments
 * @param tokens Token stream to advance
 * @param count Number of arguments to skip
 */
void	skip_processed_args(t_token **tokens, int count)
{
	while (count > 0)
	{
		*tokens = (*tokens)->next;
		count--;
	}
}

/**
 * @brief Checks if a token is a variable token
 * @param token Token string to check
 * @return 1 if token is a variable, 0 otherwise
 * 
 * A variable token starts with '$' and has at least one
 * character after it. For example: $PATH, $HOME, etc.
 */
int	is_variable_token(const char *token)
{
	return (token && token[0] == '$' && token[1] != '\0');
}
