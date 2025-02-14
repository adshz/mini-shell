/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_length_calculator.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 10:00:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 19:09:02 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"

static size_t	handle_special_char(const char *input,
			t_tokeniser_state state, size_t len)
{
	ft_putstr_fd("\nDEBUG [handle_special_char]: Checking char: [", STDERR_FILENO);
	ft_putchar_fd(input[len], STDERR_FILENO);
	ft_putstr_fd("] at position ", STDERR_FILENO);
	ft_putnbr_fd(len, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	if (state == STATE_NORMAL && len == 0 && is_special_char(input[len]))
	{
		ft_putstr_fd("DEBUG [handle_special_char]: Found special char at start\n", STDERR_FILENO);
		if (is_operator(input))
		{
			ft_putstr_fd("DEBUG [handle_special_char]: Found operator, returning 2\n", STDERR_FILENO);
			return (2);
		}
		ft_putstr_fd("DEBUG [handle_special_char]: Single special char, returning 1\n", STDERR_FILENO);
		return (1);
	}
	if (state == STATE_NORMAL && len > 0 && is_special_char(input[len]))
	{
		ft_putstr_fd("DEBUG [handle_special_char]: Found special char after content, returning ", STDERR_FILENO);
		ft_putnbr_fd(len, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		return (len);
	}
	return (0);
}

static int	handle_quote_state(t_tokeniser_state state,
			size_t len, const char *input)
{
	if ((state == STATE_IN_SINGLE_QUOTE || state == STATE_IN_DOUBLE_QUOTE) \
		&& !input[len])
	{
		ft_putendl_fd("minishell: syntax error: unclosed quotes",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

static size_t	process_char(const char *input,
			size_t len, t_tokeniser_state *state, \
						bool *prev_was_quote)
{
	size_t	special_len;
	t_tokeniser_state old_state;

	old_state = *state;
	*state = get_next_state(*state, input[len]);
	
	ft_putstr_fd("DEBUG [process_char]: State transition at pos ", STDERR_FILENO);
	ft_putnbr_fd(len, STDERR_FILENO);
	ft_putstr_fd(" char [", STDERR_FILENO);
	ft_putchar_fd(input[len], STDERR_FILENO);
	ft_putstr_fd("] from state ", STDERR_FILENO);
	ft_putnbr_fd(old_state, STDERR_FILENO);
	ft_putstr_fd(" to ", STDERR_FILENO);
	ft_putnbr_fd(*state, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

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

size_t	get_token_length_with_state(const char *input)
{
	size_t				len;
	t_tokeniser_state	state;
	bool				prev_was_quote;
	size_t				result;

	ft_putstr_fd("\nDEBUG [get_token_length]: Processing input: [", STDERR_FILENO);
	ft_putstr_fd(input, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	len = 0;
	state = STATE_NORMAL;
	prev_was_quote = false;
	while (input[len])
	{
		result = process_char(input, len, &state, &prev_was_quote);
		if (result > 0)
		{
			ft_putstr_fd("DEBUG [get_token_length]: Found token length: ", STDERR_FILENO);
			ft_putnbr_fd(result, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			return (result);
		}
		len++;
	}
	if (handle_quote_state(state, len, input))
		return (0);
	ft_putstr_fd("DEBUG [get_token_length]: Final token length: ", STDERR_FILENO);
	ft_putnbr_fd(len, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	return (len);
}
