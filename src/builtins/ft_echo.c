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

int ft_echo(t_shell *shell, t_ast_node *node)
{
    int i;
    int n_flag;

    (void)shell;
    ft_putstr_fd("Debug: Entered ft_echo function\n", 2);
    
    if (!node || !node->args)
    {
        ft_putstr_fd("Debug: node or args is NULL\n", 2);
        return (1);
    }
    
    // Debug print
    ft_putstr_fd("Debug: Arguments for echo:\n", 2);
    i = 0;
    while (node->args[i])
    {
        ft_putstr_fd("arg[", 2);
        ft_putnbr_fd(i, 2);
        ft_putstr_fd("]: '", 2);
        ft_putstr_fd(node->args[i], 2);
        ft_putstr_fd("'\n", 2);
        i++;
    }
    
    // Debug print node value
    ft_putstr_fd("Debug: Node value: '", 2);
    if (node->value)
        ft_putstr_fd(node->value, 2);
    ft_putstr_fd("'\n", 2);
    
    i = 1;  // Start from 1 to skip the command name
    n_flag = 0;
    
    // Check for -n flag
    if (node->args[i] && ft_strcmp(node->args[i], "-n") == 0)
    {
        n_flag = 1;
        i++;
    }
    
    // Print all remaining arguments
    while (node->args[i])
    {
        ft_putstr_fd(node->args[i], STDOUT_FILENO);
        if (node->args[i + 1])  // If there's another argument coming
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    
    if (!n_flag)
        ft_putchar_fd('\n', STDOUT_FILENO);
    
    ft_putstr_fd("Debug: Exiting ft_echo function\n", 2);
    return (0);
}


