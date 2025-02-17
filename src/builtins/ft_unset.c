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

#include "builtins/builtin_utils/builtin_utils.h"
#include "expander/expander.h"

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

static int	process_unset_var(t_shell *shell, const char *arg)
{
	char	*expanded_name;
	int		status;

	status = 0;
	expanded_name = expand_var_name(shell, arg);
	if (!expanded_name)
		return (0);
	if (!is_valid_identifier(expanded_name))
	{
		ft_putstr_fd("unset: '", STDERR_FILENO);
		ft_putstr_fd(expanded_name, STDERR_FILENO);
		ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
		status = 1;
	}
	else
		hashmap_remove(shell->env, expanded_name);
	free(expanded_name);
	return (status);
}

int	builtin_unset(t_shell *shell, t_ast_node *node)
{
	int		i;
	int		status;
	int		current_status;

	if (!node->args[1])
		return (0);
	i = 1;
	status = 0;
	while (node->args[i])
	{
		current_status = process_unset_var(shell, node->args[i]);
		if (current_status)
			status = current_status;
		i++;
	}
	return (status);
}
