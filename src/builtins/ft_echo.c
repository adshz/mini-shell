/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtins.h"
#include "libft.h"
#include "shell.h"
#include "parser.h"
#include <unistd.h>
#include <stdio.h>

static void ft_itoa_buf(int n, char *buf)
{
    int i;
    int is_negative;
    char tmp;
    int start;
    int end;

    i = 0;
    is_negative = 0;
    if (n < 0)
    {
        is_negative = 1;
        buf[i++] = '-';
        n = -n;
    }
    if (n == 0)
        buf[i++] = '0';
    while (n > 0)
    {
        buf[i++] = '0' + (n % 10);
        n /= 10;
    }
    buf[i] = '\0';
    
    // Reverse the digits (but not the negative sign)
    start = is_negative ? 1 : 0;
    end = i - 1;
    while (start < end)
    {
        tmp = buf[start];
        buf[start] = buf[end];
        buf[end] = tmp;
        start++;
        end--;
    }
}

static char *expand_variables(t_shell *shell, const char *arg)
{
    char *result;
    char *var_value;
    size_t i;
    size_t j;
    char var_name[1024];
    int in_single_quotes;
    int in_double_quotes;

    if (!arg)
        return (NULL);

    result = malloc(4096);  // Allocate a reasonable buffer size
    if (!result)
        return (NULL);

    i = 0;
    j = 0;
    in_single_quotes = 0;
    in_double_quotes = 0;

    while (arg[i])
    {
        if (arg[i] == '\'' && !in_double_quotes)
        {
            in_single_quotes = !in_single_quotes;
            i++;
            continue;
        }
        else if (arg[i] == '\"' && !in_single_quotes)
        {
            in_double_quotes = !in_double_quotes;
            i++;
            continue;
        }
        else if (arg[i] == '$' && !in_single_quotes)
        {
            // Handle $? special parameter
            if (arg[i + 1] == '?')
            {
                char exit_status_str[16];
                ft_itoa_buf(shell->exit_status, exit_status_str);
                ft_strlcpy(result + j, exit_status_str, ft_strlen(exit_status_str) + 1);
                j += ft_strlen(exit_status_str);
                i += 2;  // Skip $ and ?
                continue;
            }
            // Handle regular variables
            else if (arg[i + 1] && (ft_isalnum(arg[i + 1]) || arg[i + 1] == '_'))
            {
                size_t var_len = 0;
                i++;  // Skip the $
                while (arg[i + var_len] && (ft_isalnum(arg[i + var_len]) || arg[i + var_len] == '_'))
                    var_len++;
                if (var_len > 0)
                {
                    ft_strlcpy(var_name, arg + i, var_len + 1);
                    var_value = hashmap_get(shell->env, var_name);
                    if (var_value)
                    {
                        ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
                        j += ft_strlen(var_value);
                    }
                    i += var_len;
                    continue;
                }
            }
        }
        result[j++] = arg[i++];
    }
    result[j] = '\0';
    return result;
}

static bool is_valid_n_flag(const char *arg)
{
    if (!arg || !*arg || arg[0] != '-')
        return false;
    
    int i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return false;
        i++;
    }
    return i > 1;  // Must have at least one 'n'
}

int ft_echo(t_shell *shell, t_ast_node *node)
{
    int i;
    int n_flag;
    char *expanded;

    if (!node || !node->args)
        return (1);
    
    i = 1;  // Start from 1 to skip the command name
    n_flag = 0;
    
    // Check for -n flags
    while (node->args[i] && is_valid_n_flag(node->args[i]))
    {
        n_flag = 1;
        i++;
    }
    
    // Print all remaining arguments
    while (node->args[i])
    {
        expanded = expand_variables(shell, node->args[i]);
        if (expanded)
        {
            ft_putstr_fd(expanded, STDOUT_FILENO);
            free(expanded);
            if (node->args[i + 1])  // If there's another argument coming
                ft_putchar_fd(' ', STDOUT_FILENO);
        }
        i++;
    }
    
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    return (0);
}


