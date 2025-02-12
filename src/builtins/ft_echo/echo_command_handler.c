/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:49:03 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 21:34:54 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./echo.h"

void	copy_command_word(char *result, const char *value, 
			t_expansion_state *state)
{
	size_t	k;

	k = 0;
	while (value[k] && (value[k] == '"' || value[k] == '\''
			|| ft_isspace(value[k])))
		k++;
	while (value[k] && !ft_isspace(value[k])
		&& value[k] != '"' && value[k] != '\'')
		result[state->j++] = value[k++];
	while (value[k] && (value[k] == '"'
			|| value[k] == '\'' || ft_isspace(value[k])))
		k++;
	if (value[k])
	{
		result[state->j++] = ' ';
		while (value[k])
		{
			if (value[k] != '"' && value[k] != '\'')
			{
				if (ft_isspace(value[k]))
				{
					if (!state->prev_was_space)
					{
						result[state->j++] = ' ';
						state->prev_was_space = 1;
					}
				}
				else
				{
					result[state->j++] = value[k];
					state->prev_was_space = 0;
				}
			}
			k++;
		}
	}
}

void	copy_command_args(char *result,
			const char *value, t_expansion_state *state)
{
	size_t	k;

	k = 0;
	while (value[k])
	{
		if (value[k] != '"' && value[k] != '\'')
		{
			if (ft_isspace(value[k]))
			{
				if (!state->prev_was_space)
				{
					result[state->j++] = ' ';
					state->prev_was_space = 1;
				}
			}
			else
			{
				result[state->j++] = value[k];
				state->prev_was_space = 0;
			}
		}
		k++;
	}
}

void	copy_variable_value(char *result,
		const char *value, t_expansion_state *state)
{
	size_t	k;

	k = 0;
	if (value[k] == '"' || value[k] == '\'')
		k++;
	while (value[k])
	{
		if ((value[k] == '"' || value[k] == '\'') && k == ft_strlen(value) - 1)
			break ;
		if (state->in_double_quotes || state->in_single_quotes)
			result[state->j++] = value[k];
		else if (ft_isspace(value[k]))
		{
			if (!state->prev_was_space)
			{
				result[state->j++] = ' ';
				state->prev_was_space = 1;
			}
		}
		else
		{
			result[state->j++] = value[k];
			state->prev_was_space = 0;
		}
		k++;
	}
}
