/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_variable_expander.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:19:23 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:20:26 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./echo.h"

static char	*handle_special_cases(t_shell *shell, const char *arg)
{
	if (!arg)
		return (NULL);
	if (arg[0] == '~')
		return (expand_tilde(shell, arg));
	return (NULL);
}

static void	process_dollar_sign(t_shell *shell, const char *arg, \
							char *result, t_expansion_state *state)
{
	if (arg[state->i + 1] == '?')
		handle_exit_status(shell, result, state);
	else if (arg[state->i + 1] && (ft_isalnum(arg[state->i + 1]) || \
		arg[state->i + 1] == '_'))
		handle_variable(shell, arg, result, state);
	else
		result[state->j++] = arg[state->i++];
}

static void	process_current_char(t_shell *shell, const char *arg, \
								char *result, t_expansion_state *state)
{
	if (state->is_command_name && state->i == 0)
		state->i++;
	else if (arg[state->i] == '\'' || arg[state->i] == '"')
		handle_quotes(arg[state->i], state);
	else if (arg[state->i] == '$' && !state->in_single_quotes)
		process_dollar_sign(shell, arg, result, state);
	else
		handle_regular_char(arg[state->i], result, state);
}

static char	*init_expansion(void)
{
	char	*result;

	result = malloc(4096);
	if (!result)
		return (NULL);
	return (result);
}

char	*expand_complex_variable(t_shell *shell, const char *arg)
{
	t_expansion_state	state;
	char				*result;

	result = handle_special_cases(shell, arg);
	if (result)
		return (result);
	result = init_expansion();
	if (!result)
		return (NULL);
	init_expansion_state(&state, arg);
	while (arg[state.i])
		process_current_char(shell, arg, result, &state);
	return (finalize_result(result, &state));
}
