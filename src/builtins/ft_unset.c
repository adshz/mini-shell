/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"

static bool is_readonly_var(const char *var)
{
    const char *readonly_vars[] = {
        "SHLVL",
        "PWD",
        "OLDPWD",
        NULL
    };
    int i;

    i = 0;
    while (readonly_vars[i])
    {
        if (ft_strcmp((char *)var, (char *)readonly_vars[i]) == 0)
            return true;
        i++;
    }
    return false;
}

static bool is_valid_identifier(const char *str)
{
    int i;

    if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
        return false;
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return false;
        i++;
    }
    return true;
}

int builtin_unset(t_shell *shell, t_ast_node *node)
{
    int i;
    int status;

    if (!node->args[1])
        return 0;

    status = 0;
    i = 1;
    while (node->args[i])
    {
        if (!is_valid_identifier(node->args[i]))
        {
            ft_putstr_fd("unset: '", STDERR_FILENO);
            ft_putstr_fd(node->args[i], STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            status = 1;
        }
        else if (!is_readonly_var(node->args[i]))
            hashmap_remove(shell->env, node->args[i]);
        i++;
    }
    return status;
}


