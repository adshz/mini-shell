/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include "libft.h"
#include "hashtable.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

static void	update_pwd_vars(t_shell *shell, char *old_pwd)
{
	char	cwd[PATH_MAX];

	if (old_pwd)
		hashmap_set(shell->env, "OLDPWD", old_pwd);
	if (getcwd(cwd, PATH_MAX))
		hashmap_set(shell->env, "PWD", cwd);
	else
		perror("getcwd");
}

static char	*get_home_path(t_shell *shell)
{
	char	*path;

	path = hashmap_get(shell->env, "HOME");
	if (!path)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}
	return (path);
}

static char	*get_oldpwd_path(t_shell *shell)
{
	char	*path;

	path = hashmap_get(shell->env, "OLDPWD");
	if (!path)
	{
		ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
		return (NULL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	return (path);
}

static char *expand_tilde(t_shell *shell, const char *arg)
{
	char *home;

	if (arg[0] != '~')
		return (ft_strdup(arg));

	home = hashmap_get(shell->env, "HOME");
	if (!home)
	{
		ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
		return (NULL);
	}

	if (arg[1] == '\0')  // Just "~"
		return (ft_strdup(home));
	else if (arg[1] == '/') // "~/something"
		return (ft_strjoin(home, arg + 1));
	
	return (ft_strdup(arg)); // Not a tilde expansion case
}

static char *handle_path(t_shell *shell, char *arg)
{
	char *expanded;
	char *result;

	// First handle tilde expansion
	expanded = expand_tilde(shell, arg);
	if (!expanded)
		return (NULL);

	// Then handle relative paths
	if (!ft_strcmp(expanded, ".") || !ft_strcmp(expanded, "..") || 
		expanded[0] == '/' || ft_strncmp(expanded, "./", 2) == 0 || 
		ft_strncmp(expanded, "../", 3) == 0)
	{
		return expanded;
	}

	result = ft_strjoin("./", expanded);
	free(expanded);
	return result;
}

int	builtin_cd(t_shell *shell, t_ast_node *node)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	*old_pwd;

	old_pwd = getcwd(cwd, PATH_MAX);
	if (!old_pwd)
	{
		perror("cd: getcwd");
		return (1);
	}

	if (!node->args[1])
		path = get_home_path(shell);
	else if (ft_strcmp(node->args[1], "-") == 0)
		path = get_oldpwd_path(shell);
	else
		path = handle_path(shell, node->args[1]);
	if (!path)
		return (1);

	if (chdir(path) != 0)
	{
		perror("cd");
		if (path != node->args[1] && path != hashmap_get(shell->env, "HOME") 
			&& path != hashmap_get(shell->env, "OLDPWD"))
			free(path);
		return (1);
	}

	if (path != node->args[1] && path != hashmap_get(shell->env, "HOME") 
		&& path != hashmap_get(shell->env, "OLDPWD"))
		free(path);
	update_pwd_vars(shell, old_pwd);
	return (0);
}


