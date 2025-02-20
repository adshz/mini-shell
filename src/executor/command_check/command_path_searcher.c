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
	shell->exit_status = ERR_NOT_FOUND;
	return (NULL);
}

char	*try_path(const char *path, const char *cmd)
{
	char	*path_with_slash;
	char	*full_path;
	
	path_with_slash = ft_strjoin3(path, "/", NULL);
	if (!path_with_slash)
		return (NULL);
	
	full_path = ft_strjoin3(path_with_slash, cmd, NULL);
	free(path_with_slash);  // Free intermediate string
	
	if (!full_path)
		return (NULL);
	
	if (access(full_path, F_OK | X_OK) == 0)
		return (full_path);
	
	free(full_path);  // Free if path is not executable
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
	
	return (search_in_paths(shell, paths, cmd));
}
