/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:25 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 17:49:29 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins.h"
#include "expander/expander.h"
#include "utils/utils.h"
#include <stdio.h>
#include "shell.h"
#include "builtin_utils.h"

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

static void handle_expanded_value(t_shell *shell, char *key, char *value)
{
    char *expanded_value;

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

static char *handle_quoted_value(char *value)
{
    size_t val_len;
    char *unquoted;

    ft_putstr_fd("\nDEBUG [handle_quoted_value]: Input value: [", STDERR_FILENO);
    ft_putstr_fd(value, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    val_len = ft_strlen(value);
    // Only remove double quotes, preserve single quotes in all cases
    if (val_len >= 2 && value[0] == '"' && value[val_len - 1] == '"')
    {
        unquoted = ft_substr(value, 1, val_len - 2);
        ft_putstr_fd("DEBUG [handle_quoted_value]: Removed double quotes: [", STDERR_FILENO);
        ft_putstr_fd(unquoted, STDERR_FILENO);
        ft_putstr_fd("]\n", STDERR_FILENO);
        return unquoted;
    }
    // For all other cases, including single quotes, preserve as is
    unquoted = ft_strdup(value);
    ft_putstr_fd("DEBUG [handle_quoted_value]: Preserving value as is: [", STDERR_FILENO);
    ft_putstr_fd(unquoted, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);
    return unquoted;
}

static void handle_export_no_equals(t_shell *shell, char *arg)
{
    char *expanded_key;
    char *value;

    if (arg[0] == '$')
    {
        expanded_key = expand_simple_variable(shell, arg + 1);
        if (expanded_key)
        {
            value = hashmap_get(shell->env, expanded_key);
            if (value)
                hashmap_set(shell->env, expanded_key, value);
            free(expanded_key);
        }
    }
    else
    {
        value = hashmap_get(shell->env, arg);
        if (value)
            hashmap_set(shell->env, arg, value);
    }
}

static void export_with_value(t_shell *shell, char *key, char *value)
{
    char *unquoted_value;

    ft_putstr_fd("\nDEBUG [export_with_value]: Processing key: [", STDERR_FILENO);
    ft_putstr_fd(key, STDERR_FILENO);
    ft_putstr_fd("], value: [", STDERR_FILENO);
    if (value)
        ft_putstr_fd(value, STDERR_FILENO);
    else
        ft_putstr_fd("NULL", STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    if (!value)
    {
        ft_putstr_fd("DEBUG [export_with_value]: Setting empty value\n", STDERR_FILENO);
        hashmap_set(shell->env, key, "");
    }
    else
    {
        unquoted_value = handle_quoted_value(value);
        ft_putstr_fd("DEBUG [export_with_value]: After quote handling: [", STDERR_FILENO);
        ft_putstr_fd(unquoted_value, STDERR_FILENO);
        ft_putstr_fd("]\n", STDERR_FILENO);
        handle_expanded_value(shell, key, unquoted_value);
    }
}

static void process_export_argument(t_shell *shell, char *arg)
{
    char *equals;
    char *key;
    char *value;

    ft_putstr_fd("\nDEBUG [export]: Processing argument: [", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    equals = ft_strchr(arg, '=');
    if (equals)
    {
        key = ft_substr(arg, 0, equals - arg);
        value = ft_strdup(equals + 1);

        ft_putstr_fd("DEBUG [export]: Found key: [", STDERR_FILENO);
        ft_putstr_fd(key, STDERR_FILENO);
        ft_putstr_fd("], value: [", STDERR_FILENO);
        ft_putstr_fd(value, STDERR_FILENO);
        ft_putstr_fd("]\n", STDERR_FILENO);

        export_with_value(shell, key, value);

        // Debug: verify the value was stored
        char *stored_value = hashmap_get(shell->env, key);
        ft_putstr_fd("DEBUG [export]: After storing - Key: [", STDERR_FILENO);
        ft_putstr_fd(key, STDERR_FILENO);
        ft_putstr_fd("], Stored value: [", STDERR_FILENO);
        if (stored_value)
            ft_putstr_fd(stored_value, STDERR_FILENO);
        else
            ft_putstr_fd("NULL", STDERR_FILENO);
        ft_putstr_fd("]\n", STDERR_FILENO);

        free(key);
        free(value);
    }
    else if (is_valid_identifier(arg))
    {
        ft_putstr_fd("DEBUG [export]: No equals sign, handling as identifier only: [", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putstr_fd("]\n", STDERR_FILENO);
        handle_export_no_equals(shell, arg);
    }
    else
    {
        ft_putstr_fd("export: '", STDERR_FILENO);
        ft_putstr_fd(arg, STDERR_FILENO);
        ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
    }
}

static bool check_expanded_arg(t_shell *shell, char *arg)
{
    char *expanded_arg;
    bool result;

    result = true;
    if (arg[0] == '$')
    {
        expanded_arg = expand_simple_variable(shell, arg + 1);
        if (!expanded_arg || !*expanded_arg)
        {
            hashmap_iterate(shell->env, print_env_var);
            free(expanded_arg);
            result = false;
        }
        else
            free(expanded_arg);
    }
    return (result);
}

static int handle_invalid_identifier(char *arg)
{
    ft_putstr_fd("export: '", STDERR_FILENO);
    ft_putstr_fd(arg, STDERR_FILENO);
    ft_putendl_fd("': not a valid identifier", STDERR_FILENO);
    return (1);
}

int builtin_export(t_shell *shell, t_ast_node *node)
{
    int i;
    int status;

    if (!node->args[1])
    {
        hashmap_iterate(shell->env, print_env_var);
        return (0);
    }

    status = 0;
    i = 1;
    while (node->args[i])
    {
        if (!check_expanded_arg(shell, node->args[i]))
            return (0);
        if (!is_valid_identifier(node->args[i]))
            status = handle_invalid_identifier(node->args[i]);
        else
            process_export_argument(shell, node->args[i]);
        i++;
    }
    return (status);
}


