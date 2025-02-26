/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_env_conversion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:06:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:49:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"
#include "errors.h"


static char	*dup_key(t_shell *shell, char *env_line)
{
	size_t	i;

	i = 0;
	while (env_line[i])
	{
		if (env_line[i] == '=')
			return (ft_memory_collector(shell, \
							ft_substr(env_line, 0, i), false));
		i++;
	}
	return (ft_memory_collector(shell, ft_strdup(env_line), false));
}

static char	*dup_value(char *env_line)
{
	size_t	i;

	i = 0;
	while (env_line[i])
	{
		if (env_line[i] == '=')
		{
			i++;
			return (ft_memory_collector(shell, i, \
							ft_strlen(env_line) - i, false))
		}
	}
	return (NULL);
}
/**
 * @brief Converts environment array to hash table
 *
 * Creates a new hash table and populates it with environment variables.
 * Each string in envp must be in "KEY=VALUE" format. Otherwise, it is invalid.
 * Caller is responsible for freeing returned t_hashmap using hashmap_destory()
 *
 * @param envp C-like NULL-terminated array of environment strings
 * @return t_hashmap* Pointer to t_hashmap type variable, hash table containing
 * environment variable, NULL on any failure
 *
 * Example
 * @code
 * char *envp[] = {"PATH=/bin", "HOME=/home/user", NULL};
 * t_hashmap *env = env_to_hashtable(envp);
 * if (!envp)
 *   return (-1);
 * @endcode
 */
t_hashmap	*env_to_hashtable(t_shell *shell, char *envp[])
{
	t_hashmap	*table;
	int			len;
	char		*key;
	char		*value;
	int			i;

	if (!envp)
		return (NULL);
	(len = 0, i = -1);
	while (envp[len])
		len++;
	if (len)
		table = ft_memory_collector(shell, hashmap_create_table(len * 2), false);
	else
		table = ft_memory_collector(shell, hashmap_create_table(100), false);
	if (!table)
		exit(FAILURE);
	while (++i < len)
	{
		key = dup_key(envp[i]);
		value = dup_value(envp[i]);
		hashmap_insert(table, key, value, 0);
		ft_memory_delone(shell, key);
		ft_memory_delone(shell, value);
	}
	return (table);
}
