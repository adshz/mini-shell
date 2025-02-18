/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_array_expander_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:31:40 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 15:37:50 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"
#include "utils/utils.h"

int	replace_argument(char **arg, char *expanded)
{
	free(*arg);
	*arg = expanded;
	return (0);
}

char	*get_prefix(const char *arg, char *dollar_pos)
{
	size_t	prefix_len;
	char	*prefix;

	prefix_len = dollar_pos - arg;
	prefix = ft_substr(arg, 0, prefix_len);
	return (prefix);
}

char	*get_suffix(char *dollar_pos)
{
	char	*var_end;
	char	*suffix;

	if (*(dollar_pos + 1) == '?')
		return (ft_strdup(""));
	var_end = dollar_pos + 1;
	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	if (*var_end)
	{
		suffix = ft_strdup(var_end);
		return (suffix);
	}
	return (ft_strdup(""));
}

char	*combine_parts(char *prefix, char *expanded, char *suffix)
{
	char	*result;

	result = ft_strjoin3(prefix, expanded, suffix);
	free(prefix);
	free(suffix);
	free(expanded);
	return (result);
}

void	init_quote_state(t_shell *shell,
		const char *arg, bool *was_in_double_quotes)
{
	size_t	len;

	*was_in_double_quotes = shell->in_double_quotes;
	len = ft_strlen(arg);
	if (len >= 2)
	{
		if (arg[0] == '"' && arg[len - 1] == '"')
			shell->in_double_quotes = true;
		else if (arg[0] == '\'' && arg[len - 1] == '\'')
			shell->in_single_quotes = true;
	}
}
