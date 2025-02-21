/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_value_expansion.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:03:29 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 16:03:31 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "export.h"

void	export_handle_expanded_value(t_shell *shell, char *key, char *value)
{
	char	*expanded_value;

	if (value[0] == '$')
	{
		expanded_value = expand_simple_variable(shell, value + 1);
		if (expanded_value)
		{
			hashmap_set(shell->env, key, expanded_value);
			mark_env_modified(shell);
			free(expanded_value);
		}
		else
		{
			hashmap_set(shell->env, key, "");
			mark_env_modified(shell);
		}
	}
	else
	{
		hashmap_set(shell->env, key, value);
		mark_env_modified(shell);
	}
}

char	*export_handle_quoted_value(char *value)
{
	size_t	val_len;
	char	*unquoted;

	val_len = ft_strlen(value);
	if (val_len >= 2 && value[0] == '"' && value[val_len - 1] == '"')
	{
		unquoted = ft_substr(value, 1, val_len - 2);
		return (unquoted);
	}
	unquoted = ft_strdup(value);
	return (unquoted);
}

void	export_with_value(t_shell *shell, char *key, char *value)
{
	char	*unquoted_value;

	if (!value)
		hashmap_set(shell->env, key, "");
	else
	{
		unquoted_value = export_handle_quoted_value(value);
		export_handle_expanded_value(shell, key, unquoted_value);
	}
}
