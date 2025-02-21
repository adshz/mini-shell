/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_env_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:40:54 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 18:40:57 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

void	update_pwd_vars(t_shell *shell, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (old_pwd)
		hashmap_set(shell->env, "OLDPWD", old_pwd);
	else
		hashmap_set(shell->env, "OLDPWD", "");
	if (getcwd(cwd, PATH_MAX))
		hashmap_set(shell->env, "PWD", cwd);
	else
		ft_putendl_fd(ERR_GETCWD ERR_FILE, STDERR_FILENO);
	mark_env_modified(shell);
}

char	*get_oldpwd_path(t_shell *shell)
{
	char	*path;

	path = hashmap_get(shell->env, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd(ERR_OLDPWD_NOT_SET, STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (path);
}

char	*get_home_path(t_shell *shell)
{
	char	*path;

	path = hashmap_get(shell->env, "HOME");
	if (!path)
	{
		ft_putendl_fd(ERR_HOME_NOT_SET, STDERR_FILENO);
		return (NULL);
	}
	return (path);
}
