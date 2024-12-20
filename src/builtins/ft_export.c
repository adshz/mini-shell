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

static bool is_valid_identifier(const char *str)
{
    int i;

    printf("DEBUG: Validating identifier: '%s'\n", str);
    if (!str || !*str || (!ft_isalpha(*str) && *str != '_'))
    {
        printf("DEBUG: Invalid identifier - empty or starts with invalid char\n");
        return false;
    }
    i = 1;
    while (str[i] && str[i] != '=')
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
        {
            printf("DEBUG: Invalid character in identifier at position %d: '%c'\n", i, str[i]);
            return false;
        }
        i++;
    }
    printf("DEBUG: Valid identifier\n");
    return true;
}

static void export_variable(t_shell *shell, char *arg)
{
    char *equals;
    char *key;
    char *value;
    char *existing_value;

    printf("DEBUG: Exporting variable: '%s'\n", arg);
    equals = ft_strchr(arg, '=');
    if (equals)
    {
        key = ft_substr(arg, 0, equals - arg);
        value = ft_strdup(equals + 1);
        printf("DEBUG: Split into key='%s', value='%s'\n", key, value);
        if (key && value && is_valid_identifier(key))
        {
            printf("DEBUG: Setting environment variable %s=%s\n", key, value);
            hashmap_set(shell->env, key, value);
        }
        free(key);
        free(value);
    }
    else if (is_valid_identifier(arg))
    {
        existing_value = hashmap_get(shell->env, arg);
        printf("DEBUG: No '=' found, checking existing value for '%s': %s\n", 
               arg, existing_value ? existing_value : "NULL");
        if (existing_value)
            hashmap_set(shell->env, arg, ft_strdup(existing_value));
        else
            hashmap_set(shell->env, arg, ft_strdup(""));
    }
}

static char *expand_if_needed(t_shell *shell, const char *arg)
{
    char *expanded;
    char *equals;
    char *key;
    char *value;
    char *result;

    printf("DEBUG: Checking if expansion needed for: '%s'\n", arg);
    if (arg[0] != '$')
        return ft_strdup(arg);

    equals = ft_strchr(arg, '=');
    if (!equals)
        return ft_strdup(arg);

    // Extract the variable name (excluding $)
    key = ft_substr(arg + 1, 0, equals - (arg + 1));
    printf("DEBUG: Expanding variable: '%s'\n", key);

    // Get the expanded value
    expanded = expand_variables(shell, key);
    free(key);

    if (!expanded)
        return ft_strdup(arg);

    // Create the new string: expanded_value=value
    value = ft_strdup(equals + 1);
    result = ft_strjoin3(expanded, "=", value);
    printf("DEBUG: Expanded result: '%s'\n", result);

    free(expanded);
    free(value);
    return result;
}

static void print_sorted_env(t_shell *shell)
{
    // TODO: Implement printing sorted environment variables
    (void)shell;
}

int builtin_export(t_shell *shell, t_ast_node *node)
{
    int i;
    char *expanded_arg;

    if (!node->args[1])
    {
        print_sorted_env(shell);
        return 0;
    }

    i = 1;
    while (node->args[i])
    {
        printf("DEBUG: Processing export argument %d: '%s'\n", i, node->args[i]);
        expanded_arg = expand_if_needed(shell, node->args[i]);
        printf("DEBUG: After expansion: '%s'\n", expanded_arg);

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


