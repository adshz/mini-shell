/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:03:15 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 16:03:17 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "export.h"

void	handle_export_no_equals(t_shell *shell, char *arg)
{
	char	*expanded_key;
	char	*value;

	if (arg[0] == '$')
	{
		expanded_key = expand_simple_variable(shell, arg + 1);
		if (expanded_key)
		{
			value = hashmap_get(shell->env, expanded_key);
			if (value)
				hashmap_set(shell->env, expanded_key, value);
			free(expanded_key);
		}
	}
	else
	{
		value = hashmap_get(shell->env, arg);
		if (value)
			hashmap_set(shell->env, arg, value);
	}
}

void	process_export_argument(t_shell *shell, char *arg)
{
	char	*equals;
	char	*key;
	char	*value;

	equals = ft_strchr(arg, '=');
	if (equals)
	{
		key = ft_substr(arg, 0, equals - arg);
		value = ft_strdup(equals + 1);
		export_with_value(shell, key, value);
		free(key);
		free(value);
	}
	else if (is_valid_identifier(arg))
		handle_export_no_equals(shell, arg);
	else
		export_handle_invalid_identifier(arg);
}
