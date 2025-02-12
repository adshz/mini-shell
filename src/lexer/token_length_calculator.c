/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_length_calculator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/07 10:00:00 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

/**
 * @brief Handles special character cases in token length calculation
 * 
 * @param input The input string being tokenized
 * @param state Current state of the tokenizer (STATE_NORMAL, etc.)
 * @param len Current length of the token being processed
 * 
 * @return size_t Returns:
 *         - 2 if input starts with a two-character operator (e.g., ">=", "<=")
 *         - 1 if input starts with a single special character
 *         - len if a special character is found after other characters
 *         - 0 if no special character handling is needed
 * 
 * This function is part of the tokenization process and determines how special
 * characters (operators, punctuation, etc.) should affect token length calculation.
 * It handles both single-character operators and two-character operators.
 */
static size_t handle_special_char(const char *input, t_tokeniser_state state, size_t len)
{
	if (state == STATE_NORMAL && len == 0 && is_special_char(input[len]))
	{
		if (is_operator(input))
			return (2);
		return (1);
	}
	if (state == STATE_NORMAL && len > 0 && is_special_char(input[len]))
		return (len);
	return (0);
}

/**
 * @brief Handles quote state transitions and validation
 * 
 * @param state Current state of the tokenizer
 * @param len Current position in the input string
 * @param input The input string being tokenized
 * 
 * @return int Returns:
 *         - 1 if there's an unclosed quote (error)
 *         - 0 if quotes are properly closed
 * 
 * Validates quote states and reports syntax errors for unclosed quotes.
 * This function is called at the end of tokenization to ensure all
 * quotes have been properly closed.
 */
static int handle_quote_state(t_tokeniser_state state, size_t len, const char *input)
{
	if ((state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE) \
		&& !input[len])
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/**
 * @brief Processes a single character in token length calculation
 * 
 * @param input The input string being tokenized
 * @param len Current position in the input string
 * @param state Pointer to current tokenizer state
 * @param prev_was_quote Pointer to boolean tracking if previous char was a quote
 * 
 * @return size_t Returns:
 *         - Current token length if a token boundary is found
 *         - Special character length if a special token is found
 *         - 0 if character processing should continue
 * 
 * Handles character-by-character processing during tokenization, including:
 * - State transitions based on quotes
 * - Whitespace handling (considering quoted strings)
 * - Special character detection
 * - Quote tracking for consecutive quoted strings
 */
static size_t process_char(const char *input, size_t len, t_tokeniser_state *state, \
						bool *prev_was_quote)
{
	size_t special_len;

	*state = get_next_state(*state, input[len]);
	if (*state == STATE_NORMAL && ft_isspace(input[len]) && len > 0)
	{
		if (input[len + 1] && (input[len + 1] == '\'' || input[len + 1] == '"') \
			&& *prev_was_quote)
			;
		else
			return (len);
	}
	*prev_was_quote = (input[len] == '\'' || input[len] == '"');
	special_len = handle_special_char(input, *state, len);
	if (special_len > 0)
		return (special_len);
	return (0);
}

/**
 * @brief Calculates token length considering quotes and special characters
 * 
 * @param input The input string to be tokenized
 * 
 * @return size_t Returns:
 *         - Length of the next token
 *         - 0 if an error occurs (e.g., unclosed quotes)
 * 
 * Main tokenization function that:
 * - Processes input character by character
 * - Handles quoted strings (both single and double quotes)
 * - Manages state transitions
 * - Detects token boundaries
 * - Validates quote closure
 * - Handles special characters and operators
 */
size_t	get_token_length_with_state(const char *input)
{
	size_t				len;
	t_tokeniser_state	state;
	bool				prev_was_quote;
	size_t				result;

	len = 0;
	state = STATE_NORMAL;
	prev_was_quote = false;
	while (input[len])
	{
		result = process_char(input, len, &state, &prev_was_quote);
		if (result > 0)
			return (result);
		len++;
	}
	if (handle_quote_state(state, len, input))
		return (0);
	return (len);
} 