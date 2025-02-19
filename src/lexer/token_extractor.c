/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_extractor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 20:57:37 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

/**
 * @brief Initializes token extraction state
 * 
 * @param len Length of token to be extracted
 * @param extract Pointer to token extraction state
 * 
 * @return char* Returns:
 *         - Newly allocated string buffer for token
 *         - NULL if allocation fails
 * 
 * Allocates memory and initializes all counters and states
 * needed for token extraction process.
 */
static char	*init_token_extraction(size_t len, t_token_extract *extract)
{
	char	*result;

	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	extract->i = 0;
	extract->j = 0;
	extract->state = STATE_NORMAL;
	extract->prev_state = STATE_NORMAL;
	return (result);
}

/**
 * @brief Processes character based on quote state
 * 
 * @param extract Pointer to token extraction state
 * @param current_char Character being processed
 * 
 * Handles character processing within quoted strings:
 * - Copies characters inside double quotes (except the quotes themselves)
 * - Copies characters inside single quotes (except the quotes themselves)
 * - Skips quote characters
 */
static void	process_quoted_char(t_token_extract *extract, char current_char)
{
	static int	was_backslash;

	if (extract->prev_state == STATE_IN_DOUBLE_QUOTE)
	{
		if (was_backslash)
		{
			if (current_char == 'n')
			{
				extract->result[(extract->j)++] = '\\';
				extract->result[(extract->j)++] = 'n';
			}
			else
				extract->result[(extract->j)++] = current_char;
			was_backslash = 0;
		}
		else if (current_char == '\\')
			was_backslash = 1;
		else if (current_char != '"')
			extract->result[(extract->j)++] = current_char;
	}
	else if (extract->prev_state == STATE_IN_SINGLE_QUOTE && \
		current_char != '\'')
		extract->result[(extract->j)++] = current_char;
}

/**
 * @brief Processes character in normal state
 * 
 * @param extract Pointer to token extraction state
 * @param current_char Character being processed
 * 
 * Handles character processing outside of quotes:
 * - Copies normal characters
 * - Skips quote characters
 * - Only processes when both current and previous states are normal
 */
static void	process_normal_char(t_token_extract *extract, char current_char)
{
	if (extract->state == STATE_NORMAL && extract->prev_state == STATE_NORMAL \
		&& current_char != '\'' && current_char != '"')
		extract->result[(extract->j)++] = current_char;
}

/**
 * @brief Extracts token from input string handling quotes and escapes
 *
 * @param input Input string to extract from
 * @param len Length of token to extract
 * @param in_single_quotes Pointer to store if token is in single quotes
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
char	*extract_token(const char *input, size_t len, bool *in_single_quotes)
{
	t_token_extract	extract;

	if (!input)
		return (NULL);
	extract.result = init_token_extraction(len, &extract);
	if (!extract.result)
		return (NULL);
	*in_single_quotes = (len >= 2 && input[0] == '\'' && \
		input[len - 1] == '\'');
	while (extract.i < len)
	{
		extract.prev_state = extract.state;
		extract.state = get_next_state(extract.state, input[extract.i]);
		process_quoted_char(&extract, input[extract.i]);
		process_normal_char(&extract, input[extract.i]);
		extract.i++;
	}
	extract.result[extract.j] = '\0';
	if (extract.j == 0 && len > 0)
	{
		free(extract.result);
		return (NULL);
	}
	return (extract.result);
}
