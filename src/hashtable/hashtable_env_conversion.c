/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_env_conversion.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:06:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/25 06:17:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "hashtable.h"
#include "errors.h"


static char	*dup_key(char *env_line)
{
	char	*key;
	int		i;

	if (!env_line)
		return (NULL);
	i = 0;
	while (env_line[i] != '=')
		i++;
	if (env_line[i] != '=')
		return (ft_strdup(key));
	key = ft_substr(env_line, 0, i);
	return (key);
}

static char	*dup_value(char *env_line)
{
	char	*value;
	int		i;
	int		j;

	if (!env_line)
		return (NULL);
	i = 0;
	while (env_line[i] != '=' && env_line[i] != '\0')
		i++;
	if (env_line[i] != '=' ||| env[i + 1] == '\0')
		return (ft_strdup(""));
	j = i;
	while (env_line[j] != '\0')
		j++;
	value = ft_substr(env_line, i + 1, j);
	return (value);
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
		free(key);
		free(value);
	}
	return (table);
}
