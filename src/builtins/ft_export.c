/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include <stdio.h>

static bool is_valid_identifier(const char *str)
{
    int i;

    if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
        return false;
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return false;
        i++;
    }
    return true;
}

static void export_variable(t_shell *shell, char *arg)
{
    char *equals;
    char *key;
    char *value;
    char *existing_value;

    equals = ft_strchr(arg, '=');
    if (equals)
    {
        key = ft_substr(arg, 0, equals - arg);
        value = ft_strdup(equals + 1);
        if (key && value && is_valid_identifier(key))
            hashmap_set(shell->env, key, value);
        free(key);
        free(value);
    }
    else if (is_valid_identifier(arg))
    {
        existing_value = hashmap_get(shell->env, arg);
        if (existing_value)
            hashmap_set(shell->env, arg, ft_strdup(existing_value));
        else
            hashmap_set(shell->env, arg, ft_strdup(""));
    }
}

static void print_sorted_env(t_shell *shell)
{
    // TODO: Implement printing sorted environment variables
    (void)shell;
}

int builtin_export(t_shell *shell, t_ast_node *node)
{
    int i;

    if (!node->args[1])
    {
        print_sorted_env(shell);
        return 0;
    }

    i = 1;
    while (node->args[i])
    {
        if (!is_valid_identifier(node->args[i]))
        {
            ft_putstr_fd("export: '", STDERR_FILENO);
            ft_putstr_fd(node->args[i], STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            return 1;
        }
        export_variable(shell, node->args[i]);
        i++;
    }
    return 0;
}


