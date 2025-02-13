/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:53:05 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:09:27 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_utils.h"
#include "expander/expander.h"

static bool	is_readonly_var(const char *name)
{
	return (ft_strcmp(name, "PWD") == 0 || \
			ft_strcmp(name, "OLDPWD") == 0 || \
			ft_strcmp(name, "PATH") == 0);
}

static char	*expand_var_name(t_shell *shell, const char *arg)
{
	char	*expanded;

	if (!arg || !*arg)
		return (NULL);
	if (arg[0] == '$')
	{
		expanded = expand_simple_variable(shell, arg + 1);
		if (!expanded)
			return (ft_strdup(arg + 1));
		return (expanded);
	}
	return (ft_strdup(arg));
}

int	builtin_unset(t_shell *shell, t_ast_node *node)
{
	int		i;
	int		status;
	char	*expanded_name;

	if (!node->args[1])
		return (0);
	status = 0;
	i = 1;
	while (node->args[i])
	{
		expanded_name = expand_var_name(shell, node->args[i]);
		if (!expanded_name)
		{
			i++;
			continue ;
		}
		if (!is_valid_identifier(expanded_name))
		{
			ft_putstr_fd("unset: '", STDERR_FILENO);
			ft_putstr_fd(expanded_name, STDERR_FILENO);
			ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
			status = 1;
		}
		else if (!is_readonly_var(expanded_name))
			hashmap_remove(shell->env, expanded_name);
		free(expanded_name);
		i++;
	}
	return (status);
}
