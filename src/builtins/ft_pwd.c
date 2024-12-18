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

char    *ft_pwd(t_shell *shell)
{
    char cwd[PATH_MAX];
    char *pwd;

    (void)shell;
    if (getcwd(cwd, PATH_MAX) == NULL)
    {
        perror("pwd");
        return NULL;
    }
    pwd = ft_strdup(cwd);
    if (!pwd)
    {
        perror("pwd");
        return NULL;
    }
    return pwd;
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

