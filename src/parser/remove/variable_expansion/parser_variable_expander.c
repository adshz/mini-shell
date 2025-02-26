/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_variable_expander.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:18:36 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 18:19:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Handles expansion of variables in a token value
 * 
 * Processes a token value that contains variables ($ syntax):
 * - For pure variable tokens ($VAR)
 * - For mixed tokens (prefix$VAR)
 * 
 * @param shell Shell context for variable lookup
 * @param value Token value to process
 * @return Expanded string or NULL on error
 */
static char	*handle_remaining_expansion(t_shell *shell, char *expanded_value, \
									const char *dollar_pos)
{
	char	*remaining_expanded;
	char	*final_value;

	remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
	if (!remaining_expanded)
		return (expanded_value);
	final_value = ft_strjoin(expanded_value, remaining_expanded);
	free(expanded_value);
	free(remaining_expanded);
	return (final_value);
}

static char	*expand_dollar_variable(t_shell *shell, const char *value)
{
	char	*expanded_value;
	char	*dollar_pos;

	expanded_value = expand_simple_variable(shell, value + 1);
	if (!expanded_value)
		expanded_value = ft_strdup("");
	dollar_pos = ft_strchr(value + 1, '$');
	if (dollar_pos)
		expanded_value = handle_remaining_expansion(shell, expanded_value, \
											dollar_pos);
	return (expanded_value);
}

static char	*expand_mid_variable(t_shell *shell, const char *value, \
								const char *dollar_pos)
{
	char	*prefix;
	char	*remaining_expanded;
	char	*final_value;

	prefix = NULL;
	remaining_expanded = NULL;
	final_value = NULL;
	prefix = ft_substr(value, 0, dollar_pos - value);
	if (!prefix)
		return (NULL);
	remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
	if (!remaining_expanded)
	{
		free(prefix);
		return (NULL);
	}
	final_value = ft_strjoin(prefix, remaining_expanded);
	free(prefix);
	free(remaining_expanded);
	return (final_value);
}

char	*parse_handle_variable_expansion(t_shell *shell, const char *value)
{
	char	*dollar_pos;

	if (!value)
		return (NULL);
	if (value[0] == '$')
		return (expand_dollar_variable(shell, value));
	dollar_pos = ft_strchr(value, '$');
	if (!dollar_pos)
		return (ft_strdup(value));
	return (expand_mid_variable(shell, value, dollar_pos));
}
