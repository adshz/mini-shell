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
#include <unistd.h>
#include <stdio.h>

static void update_pwd_vars(t_shell *shell, char *old_pwd)
{
    char cwd[PATH_MAX];

    if (old_pwd)
        hashmap_set(shell->env, "OLDPWD", old_pwd);
    if (getcwd(cwd, PATH_MAX))
        hashmap_set(shell->env, "PWD", cwd);
}

int builtin_cd(t_shell *shell, t_ast_node *node)
{
    char *path;
    char cwd[PATH_MAX];
    char *old_pwd;

    if (!node->args[1])
    {
        path = hashmap_get(shell->env, "HOME");
        if (!path)
        {
            ft_putendl_fd("cd: HOME not set", STDERR_FILENO);
            return 1;
        }
    }
    else if (ft_strcmp(node->args[1], "-") == 0)
    {
        path = hashmap_get(shell->env, "OLDPWD");
        if (!path)
        {
            ft_putendl_fd("cd: OLDPWD not set", STDERR_FILENO);
            return 1;
        }
        ft_putendl_fd(path, STDOUT_FILENO);
    }
    else
        path = node->args[1];

    old_pwd = getcwd(cwd, PATH_MAX);
    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }
    update_pwd_vars(shell, old_pwd);
    return 0;
}


