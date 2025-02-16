/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_character_handler.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:27:58 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:28:51 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./echo.h"

void	handle_quotes(char c, t_expansion_state *state)
{
	if (c == '\'' && !state->in_double_quotes)
	{
		state->in_single_quotes = !state->in_single_quotes;
		state->i++;
	}
	else if (c == '"')
	{
		state->in_double_quotes = !state->in_double_quotes;
		state->was_quoted_var = 1;
		state->i++;
	}
}

void	handle_regular_char(char c, char *result, t_expansion_state *state)
{
	if (state->in_single_quotes || state->in_double_quotes)
	{
		result[state->j++] = c;
		state->prev_was_space = (c == ' ');
	}
	else if (ft_isspace(c))
	{
		if (!state->prev_was_space)
		{
			result[state->j++] = ' ';
			state->prev_was_space = 1;
		}
	}
	else
	{
		result[state->j++] = c;
		state->prev_was_space = 0;
	}
	state->i++;
}

void	handle_exit_status(t_shell *shell, char *result,
							t_expansion_state *state)
{
	char	exit_status_str[16];

	ft_itoa_buf(shell->exit_status, exit_status_str);
	ft_strlcpy(result + state->j, exit_status_str, \
		ft_strlen(exit_status_str) + 1);
	state->j += ft_strlen(exit_status_str);
	state->i += 2;
}
