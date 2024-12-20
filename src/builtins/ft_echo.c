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
    
    if (!node || !node->args)
        return (1);
    
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
    
    return (0);
}


