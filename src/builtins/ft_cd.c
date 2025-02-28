/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 20:35:23 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 21:15:15 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

static int	change_pwd(shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (1);
	return (hashmap_replace_value(shell, shell->env, "PWD", cwd));
}

static int	cd_home(t_shell *shell)
{
	char	*home;
	char	*old_pwd;

	old_pwd = hashmap_search(shell->env, "PWD");
	hashmap_replace_value(shell, shell->env, "OLDPWD", old_pwd);
	home = hashmap_search(shell->env, "HOME");
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == ERRNO_NONE)
		return (hashmap_replace_value(shell, shell->env, "PWD", home));
	return (1);
}

static int	cd_err_msg(char *err_msg)
{
	ft_putstr_fd("minishell: cd: '", 2);
	ft_putstr_fd(err_msg, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
}

int	builtin_cd(t_shell *shell, char *path)
{
	if (!path)
		return (cd_home(shell));
	if (chdir(path) != ERRNO_NONE)
		return (cd_err_msg(path));
	hashmap_replace_value(shell, shell->env, "OLDPWD", path);
	return (change_pwd(shell));
}
