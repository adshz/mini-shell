/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_variable_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:40:28 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 21:43:44 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./echo.h"

static size_t	get_echo_variable_name(const char *arg, size_t start, char *var_name)
{
	size_t	i;
	i = 0;

	while (arg[start + i] && (ft_isalnum(arg[start + i]) || arg[start + i] == '_'))
	{
		var_name[i] = arg[start + i];
		i++;
	}
	var_name[i] = '\0';
	return i;
}

void	handle_variable(t_shell *shell, const char *arg, char *result, t_expansion_state *state)
{
	char var_name[256];
	char *var_value;
	size_t var_len;

	ft_putstr_fd("DEBUG: Handling variable\n", STDERR_FILENO);
	state->i++;
	var_len = get_echo_variable_name(arg, state->i, var_name);
	if (var_len > 0)
	{
		var_value = hashmap_get(shell->env, var_name);
		if (var_value)
		{
			ft_putstr_fd("DEBUG: Variable value: [", STDERR_FILENO);
			ft_putstr_fd(var_value, STDERR_FILENO);
			ft_putstr_fd("]\n", STDERR_FILENO);
			state->had_var_expansion = 1;
			if (state->is_command_name)
			{
				copy_command_word(result, var_value, state);
			}
			else
				copy_variable_value(result, var_value, state);
		}
		else if (state->in_double_quotes || state->was_quoted_var)
			state->was_quoted_var = 1;
		state->i += var_len;
	}
}
