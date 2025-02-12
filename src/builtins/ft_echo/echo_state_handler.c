/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_state_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:36:53 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 21:39:33 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./echo.h"

void	init_expansion_state(t_expansion_state *state, const char *arg)
{
	state->i = 0;
	state->j = 0;
	state->in_single_quotes = 0;
	state->in_double_quotes = 0;
	state->was_quoted_var = 0;
	state->prev_was_space = 0;
	state->had_var_expansion = 0;
	state->is_command_name = (ft_strchr(arg, '$') == arg + 1);
	if (arg[0] == '"')
		state->was_quoted_var = 1;
}

char	*finalize_result(char *result, t_expansion_state *state)
{
	if (!state->had_var_expansion
		&& state->j > 0 && result[state->j - 1] == ' ')
		state->j--;
	result[state->j] = '\0';
	if (state->was_quoted_var && state->j == 0)
	{
		result[0] = '\0';
		return (result);
	}
	if (state->j == 0)
	{
		free(result);
		return (NULL);
	}
	return (result);
}
