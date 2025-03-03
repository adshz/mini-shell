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


char	*dup_key(t_shell *shell, char *env_line)
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

char	*dup_value(t_shell *shell,char *env_line)
{
	size_t	i;

	i = 0;
	while (env_line[i])
	{
		if (env_line[i] == '=')
		{
			i++;
			return (ft_memory_collector(shell, \
			ft_substr(env_line, i, \
							ft_strlen(env_line) - i), false));
		}
		i++;
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
	t_hashmap_insert_params	params;
	int			len;
	int			i;

	if (!envp)
		return (NULL);
	(len = 0, i = -1);
	while (envp[len])
		len++;
	if (len)
		params.table = hashmap_create_table(shell, len * 2);
	else
		params.table = hashmap_create_table(shell, 100);
	if (!params.table)
		exit(FAILURE);
	params.flag = 0;
	while (++i < len)
	{
		ft_putstr_fd("DEBUG [env]: Processing env var: ", 2);
		ft_putstr_fd(envp[i], 2);
		ft_putstr_fd("\n", 2);
		params.key = dup_key(shell, envp[i]);
		params.value = dup_value(shell, envp[i]);
		ft_putstr_fd("DEBUG [env]: Key: ", 2);
		ft_putstr_fd(params.key, 2);
		ft_putstr_fd(", Value: ", 2);
		if (params.value)
			ft_putstr_fd(params.value, 2);
		else
			ft_putstr_fd("(null)", 2);
		ft_putstr_fd("\n", 2);
		(void)hashmap_insert(shell, params);
		ft_memory_delone(shell, params.key);
		ft_memory_delone(shell, params.value);
	}
	return (params.table);
}
