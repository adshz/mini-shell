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
	char	*new_pwd;

	if (old_pwd)
		hashmap_set(shell->env, "OLDPWD", old_pwd);
	
	new_pwd = getcwd(cwd, PATH_MAX);
	if (new_pwd)
	{
		hashmap_set(shell->env, "PWD", new_pwd);
		if (chdir(new_pwd) != 0)
			perror("cd");
	}
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

static char	*handle_path(char *arg)
{
	if (!ft_strcmp(arg, ".") || !ft_strcmp(arg, "..") || 
		arg[0] == '/' || ft_strncmp(arg, "./", 2) == 0 || 
		ft_strncmp(arg, "../", 3) == 0)
		return (arg);
	return (ft_strjoin("./", arg));
}

int	builtin_cd(t_shell *shell, t_ast_node *node)
{
	char	*path;
	char	cwd[PATH_MAX];
	char	*old_pwd;

	// Get current directory first
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
		path = handle_path(node->args[1]);
	if (!path)
		return (1);
	old_pwd = getcwd(cwd, PATH_MAX);
	if (chdir(path) != 0)
	{
		if (path != node->args[1])
			free(path);
		perror("cd");
		return (1);
	}
	if (path != node->args[1] && path != hashmap_get(shell->env, "HOME") 
		&& path != hashmap_get(shell->env, "OLDPWD"))
		free(path);
	update_pwd_vars(shell, old_pwd);
	return (0);
}


