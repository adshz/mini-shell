/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include <limits.h>

static bool is_valid_number(const char *str)
{
    int i;

    i = 0;
    if (str[i] == '+' || str[i] == '-')
        i++;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return false;
        i++;
    }
    return true;
}

static long long ft_atoll(const char *str)
{
    long long result;
    int sign;
    int i;

    result = 0;
    sign = 1;
    i = 0;

    while (ft_isspace(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (ft_isdigit(str[i]))
    {
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return (result * sign);
}

int builtin_exit(t_shell *shell, t_ast_node *node)
{
    int status;
    long long num;

    ft_putendl_fd("exit", STDERR_FILENO);
    if (!node->args[1])
    {
        cleanup_shell(shell);
        exit(0);
    }

    if (!is_valid_number(node->args[1]))
    {
        ft_putstr_fd("exit: ", STDERR_FILENO);
        ft_putstr_fd(node->args[1], STDERR_FILENO);
        ft_putendl_fd(": numeric argument required", STDERR_FILENO);
        cleanup_shell(shell);
        exit(255);
    }

    if (node->args[2])
    {
        ft_putendl_fd("exit: too many arguments", STDERR_FILENO);
        return 1;
    }

    num = ft_atoll(node->args[1]);
    status = (unsigned char)num;
    cleanup_shell(shell);
    exit(status);
    return status;
}


