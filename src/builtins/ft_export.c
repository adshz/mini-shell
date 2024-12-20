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
    const char *start;

    if (!str || !*str)
        return false;

    // Skip quotes if present
    start = str;
    if (*start == '"' && str[ft_strlen(str) - 1] == '"')
        start++;
    else if (*start == '\'' && str[ft_strlen(str) - 1] == '\'')
        start++;

    // Empty string or empty quotes
    if (!*start || *start == '"' || *start == '\'')
        return false;

    // First character must be letter or underscore
    if (!ft_isalpha(*start) && *start != '_')
        return false;

    // Rest of the characters must be alphanumeric or underscore until '='
    i = 1;
    while (start[i] && start[i] != '=')
    {
        if (!ft_isalnum(start[i]) && start[i] != '_')
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
    char *expanded_value;

    equals = ft_strchr(arg, '=');
    if (equals)
    {
        key = ft_substr(arg, 0, equals - arg);
        value = ft_strdup(equals + 1);
        if (key && value && is_valid_identifier(key))
        {
            if (value[0] == '$')
            {
                expanded_value = expand_simple_variable(shell, value + 1);
                if (expanded_value)
                {
                    hashmap_set(shell->env, key, expanded_value);
                    free(expanded_value);
                }
                else
                    hashmap_set(shell->env, key, "");
            }
            else
                hashmap_set(shell->env, key, value);
        }
        else
        {
            ft_putstr_fd("export: '", STDERR_FILENO);
            ft_putstr_fd(arg, STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
        }
        free(key);
        free(value);
    }
    else if (is_valid_identifier(arg))
    {
        // Only mark for export, don't set any value
        // Do nothing if the variable doesn't exist
        // This matches bash behavior
    }
    else
    {
        ft_putstr_fd("export: '", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
    }
}

int builtin_export(t_shell *shell, t_ast_node *node)
{
    int i;
    int ret;

    if (!node->args[1])
    {
        hashmap_iterate(shell->env, print_env_var);
        return 0;
    }

    i = 1;
    ret = 0;
    while (node->args[i])
    {
        if (!is_valid_identifier(node->args[i]))
        {
            ft_putstr_fd("export: '", STDERR_FILENO);
            ft_putstr_fd(node->args[i], STDERR_FILENO);
            ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
            return 1;  // Stop processing and return error
        }
        export_variable(shell, node->args[i]);
        i++;
    }
    return ret;
}


