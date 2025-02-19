/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_length_calculator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 16:20:10 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 16:21:50 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

static size_t	handle_special_char(const char *input, \
								t_tokeniser_state state, size_t len)
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

static int	handle_quote_state(t_tokeniser_state state,
			size_t len, const char *input)
{
	if ((state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE) \
		&& !input[len])
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes", \
				STDERR_FILENO);
		return (1);
	}
	return (0);
}

static size_t	process_char(const char *input, size_t len, \
						t_tokeniser_state *state, bool *prev_was_quote)
{
	size_t	special_len;

	*state = get_next_state(*state, input[len]);
	if (*state == STATE_NORMAL && ft_isspace(input[len]) && len > 0)
	{
		if (input[len + 1] && (input[len + 1] == '\'' || \
			input[len + 1] == '"') && *prev_was_quote)
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
