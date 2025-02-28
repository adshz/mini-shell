/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_command_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:11:59 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:12:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./echo.h"

static void	handle_remaining_chars(char *result, const char *value, \
					t_expansion_state *state, size_t k)
{
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

void	copy_command_word(char *result, const char *value, \
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
		handle_remaining_chars(result, value, state, k);
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

static void	copy_char_with_space_handling(char *result, \
										const char *value, \
										t_expansion_state *state, \
										size_t *k)
{
	if (state->in_double_quotes || state->in_single_quotes)
		result[state->j++] = value[*k];
	else if (ft_isspace(value[*k]))
	{
		if (!state->prev_was_space)
		{
			result[state->j++] = ' ';
			state->prev_was_space = 1;
		}
	}
	else
	{
		result[state->j++] = value[*k];
		state->prev_was_space = 0;
	}
	(*k)++;
}

void	copy_variable_value(char *result, \
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
		copy_char_with_space_handling(result, value, state, &k);
	}
}
