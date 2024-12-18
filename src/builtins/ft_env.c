/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"

static void print_env_var(const char *key, const char *value)
{
    char *key_copy = ft_strdup(key);
    char *value_copy = ft_strdup(value);
    
    ft_putstr_fd(key_copy, STDOUT_FILENO);
    ft_putstr_fd("=", STDOUT_FILENO);
    ft_putendl_fd(value_copy, STDOUT_FILENO);
    
    free(key_copy);
    free(value_copy);
}

int builtin_env(t_shell *shell, t_ast_node *node)
{
    if (node->args[1])
    {
        ft_putendl_fd("env: too many arguments", STDERR_FILENO);
        return (1);
    }

    hashmap_iterate(shell->env, print_env_var);
    return (0);
}


