/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_variable.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 20:40:16 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 23:39:07 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer/lexer.h"
#include "expander/expander.h"

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
static char	*handle_dollar_var(t_shell *shell, const char *value)
{
	char	*expanded_value;
	char	*dollar_pos;
	char	*final_value;
	char	*remaining_expanded;

	expanded_value = expand_simple_variable(shell, value + 1);
	if (!expanded_value)
		expanded_value = ft_strdup("");
	dollar_pos = ft_strchr(value + 1, '$');
	if (!dollar_pos)
		return (expanded_value);
	remaining_expanded = parse_handle_variable_expansion(shell, dollar_pos);
	if (!remaining_expanded)
		return (expanded_value);
	final_value = ft_strjoin(expanded_value, remaining_expanded);
	free(expanded_value);
	free(remaining_expanded);
	return (final_value);
}

static char	*handle_mixed_var(t_shell *shell, const char *value)
{
	char	*dollar_pos;
	char	*prefix;
	char	*remaining_expanded;
	char	*final_value;

	dollar_pos = ft_strchr(value, '$');
	if (!dollar_pos)
		return (ft_strdup(value));
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
	if (!value)
		return (NULL);
	if (value[0] == '$')
		return (handle_dollar_var(shell, value));
	return (handle_mixed_var(shell, value));
}
