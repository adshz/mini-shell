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
#include <unistd.h>

static bool is_n_flag(const char *arg)
{
    int i;

    if (!arg || arg[0] != '-')
        return false;
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return false;
        i++;
    }
    return (i > 1);
}

int builtin_echo(t_shell *shell, t_ast_node *node)
{
    int i;
    bool n_flag;

    (void)shell;
    if (!node || !node->args || !node->args[1])
    {
        ft_putchar_fd('\n', STDOUT_FILENO);
        return 0;
    }
    n_flag = is_n_flag(node->args[1]);
    i = n_flag ? 2 : 1;
    while (node->args[i])
    {
        ft_putstr_fd(node->args[i], STDOUT_FILENO);
        if (node->args[i + 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);
    return 0;
}


