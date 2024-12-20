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
#include "expander.h"
#include <stdio.h>

static void print_env_var(const char *key, const char *value)
{
    ft_putstr_fd((char *)key, STDOUT_FILENO);
    if (value)
    {
        ft_putchar_fd('=', STDOUT_FILENO);
        ft_putstr_fd((char *)value, STDOUT_FILENO);
    }
    ft_putchar_fd('\n', STDOUT_FILENO);
}

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
        value = hashmap_get(shell->env, arg);
        if (value)
            hashmap_set(shell->env, arg, ft_strdup(value));
    }
}

int builtin_export(t_shell *shell, t_ast_node *node)
{
    int i;
    char *expanded_arg;

    if (!node->args[1])
    {
        hashmap_iterate(shell->env, print_env_var);
        return 0;
    }

    i = 1;
    while (node->args[i])
    {
        expanded_arg = expand_simple_variable(shell, node->args[i]);
        if (!expanded_arg)
        {
            hashmap_iterate(shell->env, print_env_var);
            return 0;
        }

        if (!is_valid_identifier(expanded_arg))
        {
            ft_putstr_fd("export: '", STDERR_FILENO);
            ft_putstr_fd(node->args[i], STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            free(expanded_arg);
            return 1;
        }
        export_variable(shell, expanded_arg);
        free(expanded_arg);
        i++;
    }
    return 0;
}


