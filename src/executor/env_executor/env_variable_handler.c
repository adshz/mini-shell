/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_handler.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:30:28 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 14:35:41 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "hashtable/hashtable.h"

int	handle_variable_assignment(t_shell *shell, const char *assignment)
{
	char	*equals;
	char	*key;
	char	*value;

	equals = ft_strchr(assignment, '=');
	if (!equals)
		return (1);
	key = ft_substr(assignment, 0, equals - assignment);
	value = ft_strdup(equals + 1);
	if (!key || !value)
	{
		free(key);
		free(value);
		return (1);
	}
	hashmap_set(shell->env, key, value);
	mark_env_modified(shell);
	free(key);
	free(value);
	return (0);
}
