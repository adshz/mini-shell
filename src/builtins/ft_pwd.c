/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
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
#include <unistd.h>
#include <stdio.h>
#include <limits.h>

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

static char	*get_pwd_from_cwd(t_shell *shell)
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
	hashmap_set(shell->env, "PWD", pwd);
	return (pwd);
}

char	*ft_pwd(t_shell *shell)
{
	char	*pwd;

	pwd = get_pwd_from_env(shell);
	if (pwd)
		return (pwd);
	return (get_pwd_from_cwd(shell));
}

int    builtin_pwd(t_shell *shell, t_ast_node *node)
{
    char    *pwd;

    (void)node;  // We don't need the node parameter for pwd
    pwd = ft_pwd(shell);
    if (!pwd)
        return 1;
    ft_putendl_fd(pwd, STDOUT_FILENO);
    free(pwd);
    return 0;
}

