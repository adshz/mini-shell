/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_array_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:53:24 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 16:01:20 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

static int	expand_single_variable(t_shell *shell,
		char **arg, bool *was_in_double_quotes)
{
	char	*prefix;
	char	*suffix;
	char	*expanded;
	char	*result;

	prefix = get_prefix(*arg, ft_strchr(*arg, '$'));
	suffix = get_suffix(ft_strchr(*arg, '$'));
	if (handle_expansion(shell, ft_strchr(*arg, '$'), &expanded, was_in_double_quotes))
	{
		free(prefix);
		free(suffix);
		return (SHELL_ERROR);
	}
	result = combine_parts(prefix, expanded, suffix);
	return (replace_argument(arg, result));
}

static int	process_remaining_variables(t_shell *shell, char **result)
{
	char	*temp_suffix;

	temp_suffix = *result;
	if (ft_strchr(temp_suffix, '$'))
	{
		if (process_single_arg_expansion(shell, &temp_suffix))
		{
			free(*result);
			return (SHELL_ERROR);
		}
		*result = temp_suffix;
	}
	return (0);
}

int	process_single_arg_expansion(t_shell *shell, char **arg)
{
	bool	was_in_double_quotes;
	int		ret;

	init_quote_state(shell, *arg, &was_in_double_quotes);
	if (!ft_strchr(*arg, '$'))
	{
		shell->in_double_quotes = was_in_double_quotes;
		return (0);
	}
	ret = expand_single_variable(shell, arg, &was_in_double_quotes);
	if (ret != 0)
		return (ret);
	ret = process_remaining_variables(shell, arg);
	shell->in_double_quotes = was_in_double_quotes;
	return (ret);
}
