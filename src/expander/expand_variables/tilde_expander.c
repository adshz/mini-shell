/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tilde_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:56:15 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:00:30 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander/expander.h"

static char	*handle_home_expansion(t_shell *shell)
{
	char	*home;

	home = hashmap_get(shell->env, "HOME");
	if (!home)
		return (NULL);
	return (ft_strdup(home));
}

static char	*handle_path_expansion(char *home, const char *arg)
{
	if (arg[1] == '\0')
		return (ft_strdup(home));
	else if (arg[1] == '/')
		return (ft_strjoin(home, arg + 1));
	return (ft_strdup(arg));
}

char	*expand_tilde(t_shell *shell, const char *arg)
{
	char	*home;

	if (!arg || arg[0] != '~')
		return (ft_strdup(arg));
	home = handle_home_expansion(shell);
	if (!home)
		return (ft_strdup(arg));
	return (handle_path_expansion(home, arg));
}
