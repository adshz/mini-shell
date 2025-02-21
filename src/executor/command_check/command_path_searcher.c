/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_searcher.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:55:29 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:57:06 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../executor.h"
#include "libft.h"
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

char	*try_path(const char *path, const char *cmd)
{
	char		*path_with_slash;
	char		*full_path;

	path_with_slash = ft_strjoin(path, "/");
	if (!path_with_slash)
		return (NULL);
	full_path = ft_strjoin(path_with_slash, cmd);
	free(path_with_slash);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK) != 0 || access(full_path, X_OK) != 0)
	{
		free(full_path);
		return (NULL);
	}
	return (full_path);
}

char	*search_in_paths(t_shell *shell, char **paths, const char *cmd)
{
	int		i;
	char	*full_path;

	i = 0;
	while (paths[i])
	{
		full_path = try_path(paths[i], cmd);
		if (full_path)
		{
			shell->exit_status = 0;
			ft_free_array(paths);
			return (full_path);
		}
		i++;
	}
	shell->exit_status = ERR_NOT_FOUND;
	ft_free_array(paths);
	return (NULL);
}

static char	**get_path_directories(t_hashmap *env)
{
	char	*path_var;
	char	**paths;

	path_var = hashmap_get(env, "PATH");
	if (!path_var || !*path_var)
		path_var = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin";
	paths = ft_split(path_var, ':');
	if (!paths)
		return (NULL);
	return (paths);
}

char	*get_command_path(t_shell *shell, const char *cmd, t_hashmap *env)
{
	char	**paths;
	char	*path;

	if (!cmd || !*cmd)
	{
		shell->exit_status = ERR_NOT_FOUND;
		return (NULL);
	}
	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
	paths = get_path_directories(env);
	if (!paths)
	{
		shell->exit_status = ERR_NOT_FOUND;
		return (NULL);
	}
	path = search_in_paths(shell, paths, cmd);
	return (path);
}
