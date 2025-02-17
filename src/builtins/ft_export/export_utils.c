/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:04:19 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 16:04:21 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "export.h"

bool	export_check_expanded_arg(t_shell *shell, char *arg)
{
	char	*expanded_arg;
	bool	result;

	result = true;
	if (arg[0] == '$')
	{
		expanded_arg = expand_simple_variable(shell, arg + 1);
		if (!expanded_arg || !*expanded_arg)
		{
			hashmap_iterate(shell->env, export_print_env_var);
			free(expanded_arg);
			result = false;
		}
		else
			free(expanded_arg);
	}
	return (result);
}

int	export_handle_invalid_identifier(char *arg)
{
	ft_putstr_fd("export: '", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
	return (1);
}

void	export_print_env_var(const char *key, const char *value)
{
	ft_putstr_fd((char *)key, STDOUT_FILENO);
	if (value)
	{
		ft_putchar_fd('=', STDOUT_FILENO);
		ft_putstr_fd((char *)value, STDOUT_FILENO);
	}
	ft_putchar_fd('\n', STDOUT_FILENO);
}
