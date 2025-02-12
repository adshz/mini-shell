/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extractor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/07 10:00:00 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

/**
 * @brief Initializes token extraction state
 * 
 * @param len Length of token to be extracted
 * @param i Pointer to input string position counter
 * @param j Pointer to result string position counter
 * @param state Pointer to current tokenizer state
 * @param prev_state Pointer to previous tokenizer state
 * 
 * @return char* Returns:
 *         - Newly allocated string buffer for token
 *         - NULL if allocation fails
 * 
 * Allocates memory and initializes all counters and states
 * needed for token extraction process.
 */
static char *init_token_extraction(size_t len, size_t *i, size_t *j, \
								t_tokeniser_state *state, t_tokeniser_state *prev_state)
{
	char *result;

	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	*i = 0;
	*j = 0;
	*state = STATE_NORMAL;
	*prev_state = STATE_NORMAL;
	return (result);
}

/**
 * @brief Processes character based on quote state
 * 
 * @param result Output string buffer
 * @param j Pointer to current position in result buffer
 * @param current_char Character being processed
 * @param prev_state Previous tokenizer state
 * 
 * Handles character processing within quoted strings:
 * - Copies characters inside double quotes (except the quotes themselves)
 * - Copies characters inside single quotes (except the quotes themselves)
 * - Skips quote characters
 */
static void process_quoted_char(char *result, size_t *j, char current_char, \
							t_tokeniser_state prev_state)
{
	if (prev_state == STATE_IN_DOUBLE_QUOTE && current_char != '"')
		result[(*j)++] = current_char;
	else if (prev_state == STATE_IN_SINGLE_QUOTE && current_char != '\'')
		result[(*j)++] = current_char;
}

/**
 * @brief Processes character in normal state
 * 
 * @param result Output string buffer
 * @param j Pointer to current position in result buffer
 * @param current_char Character being processed
 * @param state Current tokenizer state
 * @param prev_state Previous tokenizer state
 * 
 * Handles character processing outside of quotes:
 * - Copies normal characters
 * - Skips quote characters
 * - Only processes when both current and previous states are normal
 */
static void process_normal_char(char *result, size_t *j, char current_char, \
							t_tokeniser_state state, t_tokeniser_state prev_state)
{
	if (state == STATE_NORMAL && prev_state == STATE_NORMAL && \
		current_char != '\'' && current_char != '"')
		result[(*j)++] = current_char;
}

/**
 * @brief Extracts token from input string handling quotes and escapes
 *
 * @param input Input string to extract from
 * @param len Length of token to extract
 * 
 * @return char* Returns:
 *         - Newly allocated string containing the extracted token
 *         - NULL if memory allocation fails
 *
 * Extraction process:
 * 1. Allocates space for token
 * 2. Copies characters while tracking state
 * 3. Handles quoted strings (both single and double quotes)
 * 4. Removes quotes from final token
 * 5. Maintains state tracking for proper quote handling
 *
 * @note Caller must free returned string
 * @see get_next_state() for state tracking details
 */
char	*extract_token(const char *input, size_t len)
{
	char				*result;
	size_t				i;
	size_t				j;
	t_tokeniser_state	state;
	t_tokeniser_state	prev_state;

	result = init_token_extraction(len, &i, &j, &state, &prev_state);
	if (!result)
		return (NULL);
	while (i < len)
	{
		prev_state = state;
		state = get_next_state(state, input[i]);
		process_quoted_char(result, &j, input[i], prev_state);
		process_normal_char(result, &j, input[i], state, prev_state);
		i++;
	}
	result[j] = '\0';
	return (result);
} 
