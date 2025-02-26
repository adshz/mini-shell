/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_array_handler_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:41:11 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:48:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	convert_exit_status_to_str(t_shell *shell, char **expanded)
{
	char	exit_str[12];

	ft_itoa_buf(shell->exit_status, exit_str);
	*expanded = ft_strdup(exit_str);
	return (*expanded != NULL);
}

int	process_variable_expansion(t_shell *shell,
		char *dollar_pos, char **expanded)
{
	if (dollar_pos[1] == '?')
	{
		return (convert_exit_status_to_str(shell, expanded));
	}
	*expanded = expand_simple_variable(shell, dollar_pos + 1);
	return (*expanded != NULL);
}

int	handle_expansion(t_shell *shell, char *dollar_pos,
		char **expanded, bool *was_in_double_quotes)
{
	if (!process_variable_expansion(shell, dollar_pos, expanded))
	{
		shell->in_double_quotes = *was_in_double_quotes;
		return (1);
	}
	return (0);
}
