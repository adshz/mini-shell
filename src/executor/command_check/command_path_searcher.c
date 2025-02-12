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
			ft_free_array(paths);
			shell->exit_status = 0;
			return (full_path);
		}
		i++;
	}
	ft_free_array(paths);
	shell->exit_status = 127;
	return (NULL);
}

char	*try_path(const char *path, const char *cmd)
{
	char	*full_path;

	full_path = ft_strjoin3(path, "/", cmd);
	if (full_path && access(full_path, X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

static char	**get_path_directories(t_hashmap *env)
{
	char	*path_var;
	char	**paths;

	path_var = hashmap_get(env, "PATH");
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	return (paths);
}

char	*get_command_path(t_shell *shell, const char *cmd, t_hashmap *env)
{
	char	**paths;

	if (is_special_path(cmd))
		return (NULL);
	paths = get_path_directories(env);
	if (!paths)
	{
		shell->exit_status = 127;
		return (NULL);
	}
	return (search_in_paths(shell, paths, cmd));
}
