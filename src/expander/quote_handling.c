/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:07:11 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:16:05 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

int	is_quoted(const char *str)
{
	return (str && (str[0] == '\'' || str[0] == '"'));
}

static void	handle_string_copy(t_copy_state *state, char quote_char)
{
	if (quote_char)
	{
		state->i++;
		while (state->str[state->i] && state->str[state->i] != quote_char)
		{
			state->result[state->j] = state->str[state->i];
			state->j++;
			state->i++;
		}
		if (state->str[state->i])
			state->i++;
	}
	else
	{
		state->result[state->j] = state->str[state->i];
		state->j++;
		state->i++;
	}
}

static char	*process_string(const char *str)
{
	t_copy_state	state;

	state.result = malloc(ft_strlen(str) + 1);
	if (!state.result)
		return (NULL);
	state.str = str;
	state.i = 0;
	state.j = 0;
	while (str[state.i])
	{
		if (str[state.i] == '\'' || str[state.i] == '"')
			handle_string_copy(&state, str[state.i]);
		else
			handle_string_copy(&state, 0);
	}
	state.result[state.j] = '\0';
	return (state.result);
}

char	*strip_quotes(char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = process_string(str);
	free(str);
	return (result);
}
