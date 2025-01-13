/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:40 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:45:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_utils.h"
#include "hashtable.h"
#include "libft.h"
#include "shell.h"
#include <stdlib.h>
#include <unistd.h>

t_hashmap    *init_hashmap(void)
{
    return hashmap_create_table(HASH_SIZE);
}

t_hashmap    *init_env(char **envp)
{
    if (!envp)
        return hashmap_create_table(HASH_SIZE);
    return env_to_hash(envp);
}

void    init_pwd(t_shell *shell)
{
    char    *tmp;

    if (!hashmap_search(shell->env, "PWD"))
    {
        tmp = getcwd(NULL, 0);
        if (!tmp)
            return;
        hashmap_insert(shell->env, "PWD", tmp, 0);
        free(tmp);
    }
}

void    init_shlvl(t_shell *shell)
{
    char    *tmp;
    int     shlvl;

    shlvl = 1;
    tmp = hashmap_search(shell->env, "SHLVL");
    if (tmp && ft_atoi(tmp) > 0)
        shlvl = ft_atoi(tmp) + 1;
    tmp = ft_itoa(shlvl);
    if (!tmp)
        return;
    hashmap_insert(shell->env, "SHLVL", tmp, 0);
    free(tmp);
}

void    init_env_vars(t_shell *shell, char *argv[])
{
    init_pwd(shell);
    init_shlvl(shell);
    if (!hashmap_search(shell->env, "PATH"))
        hashmap_insert(shell->env, "PATH",
            "/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", 0);
    if (!hashmap_search(shell->env, "_"))
        hashmap_insert(shell->env, "_", argv[0], 0);
    hashmap_remove(shell->env, "OLDPWD");
} 