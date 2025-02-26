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
	if (handle_expansion(shell, ft_strchr(*arg, '$'), \
					&expanded, was_in_double_quotes))
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

static int	check_token_validity(t_shell *shell, const char *arg,
		bool *was_in_double_quotes, t_token **current_token)
{
	*current_token = shell->tokens;
	if (*current_token && ft_strcmp((*current_token)->value, arg) == 0)
	{
		shell->in_double_quotes = *was_in_double_quotes;
		shell->in_single_quotes = false;
		return (1);
	}
	while (*current_token && ft_strcmp((*current_token)->value, arg) != 0)
		*current_token = (*current_token)->next;
	if (!ft_strchr(arg, '$') || \
		(*current_token && (*current_token)->in_single_quotes))
	{
		shell->in_double_quotes = *was_in_double_quotes;
		shell->in_single_quotes = false;
		return (1);
	}
	return (0);
}

int	process_single_arg_expansion(t_shell *shell, char **arg)
{
	bool	was_in_double_quotes;
	int		ret;
	t_token	*current_token;

	was_in_double_quotes = false;
	ret = 0;
	init_quote_state(shell, *arg, &was_in_double_quotes);
	if (check_token_validity(shell, *arg, &was_in_double_quotes, \
						&current_token))
		return (0);
	ret = expand_single_variable(shell, arg, &was_in_double_quotes);
	if (ret != 0)
		return (ret);
	ret = process_remaining_variables(shell, arg);
	shell->in_double_quotes = was_in_double_quotes;
	shell->in_single_quotes = false;
	return (ret);
}
