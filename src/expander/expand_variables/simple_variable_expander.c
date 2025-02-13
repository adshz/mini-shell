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

	ft_putstr_fd("\nDEBUG: Extracting variable name from: [", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	i = 0;
	j = 0;
	while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
		var_name[j++] = arg[i++];
	var_name[j] = '\0';
	ft_putstr_fd("DEBUG: Extracted variable name: [", STDERR_FILENO);
	ft_putstr_fd(var_name, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (i);
}

static char	*clean_variable_value(const char *var_value, t_shell *shell)
{
	char	*cleaned;

	ft_putstr_fd("\nDEBUG: Cleaning variable value: [", STDERR_FILENO);
	ft_putstr_fd(var_value, STDERR_FILENO);
	ft_putstr_fd("], in_double_quotes: ", STDERR_FILENO);
	ft_putstr_fd(shell->in_double_quotes ? "true" : "false", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	cleaned = ft_strdup(var_value);
	ft_putstr_fd("DEBUG: Preserving value as stored in env: [", STDERR_FILENO);
	ft_putstr_fd(cleaned, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (cleaned);
}

static char	*get_variable_value(t_shell *shell, const char *var_name)
{
	char	*var_value;
	char	*cleaned_value;

	ft_putstr_fd("\nDEBUG: Looking up variable in env: [", STDERR_FILENO);
	ft_putstr_fd(var_name, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	var_value = hashmap_get(shell->env, var_name);
	if (!var_value)
	{
		ft_putstr_fd("DEBUG: Variable not found in env\n", STDERR_FILENO);
		return (NULL);
	}
	ft_putstr_fd("DEBUG: Found raw value: [", STDERR_FILENO);
	ft_putstr_fd(var_value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	cleaned_value = clean_variable_value(var_value, shell);
	ft_putstr_fd("DEBUG: Returning cleaned value: [", STDERR_FILENO);
	if (cleaned_value)
		ft_putstr_fd(cleaned_value, STDERR_FILENO);
	else
		ft_putstr_fd("NULL", STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (cleaned_value);
}

char	*expand_simple_variable(t_shell *shell, const char *arg)
{
	char	var_name[1024];
	char	*value;

	ft_putstr_fd("\nDEBUG: expand_simple_variable called with arg: [", STDERR_FILENO);
	if (arg)
		ft_putstr_fd(arg, STDERR_FILENO);
	else
		ft_putstr_fd("NULL", STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	if (!arg)
	{
		ft_putstr_fd("DEBUG: NULL argument, returning NULL\n", STDERR_FILENO);
		return (NULL);
	}
	extract_variable_name(arg, var_name);
	value = get_variable_value(shell, var_name);
	ft_putstr_fd("DEBUG: Final expansion result: [", STDERR_FILENO);
	if (value)
		ft_putstr_fd(value, STDERR_FILENO);
	else
		ft_putstr_fd("NULL", STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	return (value);
}

char	*expand_variable(t_shell *shell, const char *var_name)
{
	return (expand_simple_variable(shell, var_name));
}
