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
	ft_putstr_fd("\nDEBUG get_prefix: [", STDERR_FILENO);
	if (prefix)
		ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (prefix);
}

char	*get_suffix(char *dollar_pos)
{
	char	*var_end;
	char	*suffix;

	if (*(dollar_pos + 1) == '?')
		return (ft_strdup(""));
	var_end = dollar_pos + 1;
	ft_putstr_fd("DEBUG get_suffix - variable name starts with: [",
			STDERR_FILENO);
	ft_putstr_fd(var_end, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	while (*var_end && (ft_isalnum(*var_end) || *var_end == '_'))
		var_end++;
	ft_putstr_fd("DEBUG get_suffix - stopped at: [", STDERR_FILENO);
	ft_putstr_fd(var_end, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	if (*var_end)
	{
		suffix = ft_strdup(var_end);
		ft_putstr_fd("DEBUG get_suffix - returning: [", STDERR_FILENO);
		ft_putstr_fd(suffix, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		return (suffix);
	}
	return (ft_strdup(""));
}

char	*combine_parts(char *prefix, char *expanded, char *suffix)
{
	char	*result;

	ft_putstr_fd("\nDEBUG combine_parts:\n", STDERR_FILENO);
	ft_putstr_fd("  prefix: [", STDERR_FILENO);
	if (prefix)
		ft_putstr_fd(prefix, STDERR_FILENO);
	ft_putstr_fd("]\n  expanded: [", STDERR_FILENO);
	if (expanded)
		ft_putstr_fd(expanded, STDERR_FILENO);
	ft_putstr_fd("]\n  suffix: [", STDERR_FILENO);
	if (suffix)
		ft_putstr_fd(suffix, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	result = ft_strjoin3(prefix, expanded, suffix);
	ft_putstr_fd("DEBUG combine_parts - result: [", STDERR_FILENO);
	if (result)
		ft_putstr_fd(result, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	free(prefix);
	free(suffix);
	free(expanded);
	return (result);
}

void	init_quote_state(t_shell *shell,
		const char *arg, bool *was_in_double_quotes)
{
	*was_in_double_quotes = shell->in_double_quotes;
	if (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"')
		shell->in_double_quotes = true;
}
