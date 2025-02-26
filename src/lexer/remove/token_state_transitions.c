/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_state_transitions.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:35:07 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 20:55:11 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Handles transitions from normal state
 *
 * @param c Current character
 * @return Next state based on character
 */
t_tokeniser_state	handle_normal_state(char c)
{
	if (c == '\\')
		return (STATE_IN_BACKSLASH);
	if (c == '\'')
		return (STATE_IN_SINGLE_QUOTE);
	if (c == '\"')
		return (STATE_IN_DOUBLE_QUOTE);
	return (STATE_NORMAL);
}

/**
 * @brief Handles transitions from double quote state
 *
 * @param c Current character
 * @param was_backslash Pointer to backslash state
 * @return Next state based on character and backslash state
 */
t_tokeniser_state	handle_double_quote_state(char c, \
												int *was_backslash)
{
	if (c == '\\' && !(*was_backslash))
	{
		*was_backslash = 1;
		return (STATE_IN_DOUBLE_QUOTE);
	}
	if (c == '\"' && !(*was_backslash))
		return (STATE_NORMAL);
	*was_backslash = 0;
	return (STATE_IN_DOUBLE_QUOTE);
}

/**
 * @brief Determines next tokenizer state based on current state and character
 *
 * Handles state transitions for:
 * - Backslash escaping
 * - Single quotes
 * - Double quotes
 * - Normal state
 *
 * State machine logic:
 * - Backslash -> Normal (after processing escaped char)
 * - Normal -> Backslash/Single/Double quote states
 * - Quote states -> Normal (when matching quote found)
 *
 * @param current_state Current tokenizer state
 * @param c            Current character being processed
 * @return Next state for tokenizer
 *
 * @note Maintains backslash state between calls
 * @see t_tokenizer_state for possible states
 */
t_tokeniser_state	get_next_state(t_tokeniser_state current_state, char c)
{
	static int	was_backslash;

	if (current_state == STATE_IN_BACKSLASH)
	{
		was_backslash = 0;
		return (STATE_NORMAL);
	}
	if (current_state == STATE_NORMAL)
		return (handle_normal_state(c));
	if (current_state == STATE_IN_DOUBLE_QUOTE)
		return (handle_double_quote_state(c, &was_backslash));
	if (current_state == STATE_IN_SINGLE_QUOTE && c == '\'')
		return (STATE_NORMAL);
	return (current_state);
}
