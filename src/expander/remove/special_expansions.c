/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   special_expansions.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:16:37 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:19:28 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"
#include "libft.h"

char	*expand_exit_status(t_shell *shell)
{
	return (ft_itoa(shell->exit_status));
}

char	*expand_home_dir(t_shell *shell, const char *arg)
{
	char	*home;

	if (arg[0] != '~')
		return (ft_strdup(arg));
	home = hashmap_get(shell->env, "HOME");
	if (!home)
		return (ft_strdup(arg));
	if (arg[1] == '\0')
		return (ft_strdup(home));
	else if (arg[1] == '/')
		return (ft_strjoin(home, arg + 1));
	return (ft_strdup(arg));
}
