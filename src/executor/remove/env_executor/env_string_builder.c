/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_string_builder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:58:46 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:59:19 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	validate_env_string_input(const char *key, const char *value, \
							size_t *key_len, size_t *value_len)
{
	if (!key || !value)
	{
		ft_putendl_fd("NULL key or value in create_env_string", STDERR_FILENO);
		return (0);
	}
	*key_len = ft_strlen(key);
	*value_len = ft_strlen(value);
	if (*key_len > SIZE_MAX - 2 || *key_len + 2 > SIZE_MAX - *value_len)
	{
		ft_putendl_fd("String length overflow in create_env_string", \
			STDERR_FILENO);
		return (0);
	}
	return (1);
}

char	*build_env_string(const char *key, const char *value,
		size_t key_len, size_t value_len)
{
	char	*env_str;
	size_t	total_len;

	total_len = key_len + value_len + 2;
	env_str = malloc(total_len);
	if (!env_str)
	{
		ft_putendl_fd("malloc failed in create_env_string", STDERR_FILENO);
		return (NULL);
	}
	ft_strlcpy(env_str, key, key_len + 1);
	env_str[key_len] = '=';
	ft_strlcpy(env_str + key_len + 1, value, value_len + 1);
	return (env_str);
}

char	*create_env_string(const char *key, const char *value)
{
	size_t	key_len;
	size_t	value_len;

	key_len = 0;
	value_len = 0;
	if (!validate_env_string_input(key, value, &key_len, &value_len))
		return (NULL);
	return (build_env_string(key, value, key_len, value_len));
}
