/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:47:45 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:48:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_path	get_path(t_shell *shell, char *cmd)
{
	char	*path_env_value;

	if (*cmd == '\0')
		return ((t_path){\
			(t_err){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, cmd}, NULL});
	if (ft_strnstr(cmd, "/", ft_strlen(cmd)))
		return ((t_path){check_executable(cmd, false), cmd});
	path_env_value = hashmap_search(shell->env, "PATH");
	if (path_env_value)
		return (get_env_path(shell, path_env_value, cmd));
	return ((t_path){(t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, cmd}, NULL});
}

t_path	get_env_path(t_shell *shell, char *path, char *cmd)
{
	size_t	i;
	t_err	err;
	char	*cmd_path;
	char	**split_path;

	split_path = ft_split(path, ':');
	i = 0;
	while (split_path[i])
	{
		cmd_path = ft_memory_collector(shell, \
								ft_strjoin_with_char_free_both(\
									ft_strdup(split_path[i]), ft_strdup(cmd),\
									'/'), false);
		err = check_executable(cmd_path, true);
		if (err.num == ERRNO_NONE)
		{
			ft_free_2d_array(split_path);
			return ((t_path){(t_err){ERRNO_NONE, 42, cmd}, cmd_path});
		}
		i++;
	}
	ft_free_2d_array(split_path);
	return ((t_path){(t_err){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, cmd}, NULL});
}
