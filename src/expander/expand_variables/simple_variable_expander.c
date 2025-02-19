/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_variable_expander.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:47:40 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 17:55:24 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../expander.h"

static size_t	extract_variable_name(const char *arg, char *var_name)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
		var_name[j++] = arg[i++];
	var_name[j] = '\0';
	return (i);
}

static char	*clean_variable_value(const char *var_value)
{
	char	*cleaned;

	cleaned = ft_strdup(var_value);
	return (cleaned);
}

static char	*get_variable_value(t_shell *shell, const char *var_name)
{
	char	*var_value;
	char	*cleaned_value;

	var_value = hashmap_get(shell->env, var_name);
	if (!var_value)
		return (ft_strdup(""));
	cleaned_value = clean_variable_value(var_value);
	return (cleaned_value);
}

char	*expand_simple_variable(t_shell *shell, const char *arg)
{
	char	var_name[1024];
	char	*value;

	if (!arg)
		return (NULL);
	extract_variable_name(arg, var_name);
	value = get_variable_value(shell, var_name);
	return (value);
}

char	*expand_variable(t_shell *shell, const char *var_name)
{
	return (expand_simple_variable(shell, var_name));
}
