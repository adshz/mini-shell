/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:03 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:05:51 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

static char	*get_pwd_from_cwd(void)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("pwd");
		return (NULL);
	}
	pwd = ft_strdup(cwd);
	if (!pwd)
	{
		perror("pwd");
		return (NULL);
	}
	return (pwd);
}

static char	*get_pwd_from_env(t_shell *shell)
{
	char	*pwd;
	char	*pwd_dup;

	pwd = hashmap_get(shell->env, "PWD");
	if (!pwd)
		return (NULL);
	pwd_dup = ft_strdup(pwd);
	if (!pwd_dup)
		perror("pwd");
	return (pwd_dup);
}

char	*ft_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = get_pwd_from_cwd();
	if (pwd)
		return (pwd);
	return (get_pwd_from_env(shell));
}

int	builtin_pwd(t_shell *shell, t_ast_node *node)
{
	char	*pwd;

	(void)node;
	pwd = ft_pwd(shell);
	if (!pwd)
		return (1);
	ft_putendl_fd(pwd, STDOUT_FILENO);
	free(pwd);
	return (0);
}
