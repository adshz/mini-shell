/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include "libft.h"

int    builtin_exit(t_shell *shell, t_ast_node *node)
{
    int    exit_code;

    if (!shell || !node)
        return (1);

    if (!shell->in_pipe)
        ft_putendl_fd("exit", STDERR_FILENO);
    
    // Get exit code before cleanup
    if (!node->args || !node->args[1])
        exit_code = shell->exit_status;
    else if (!ft_isnumber(node->args[1]))
    {
        print_error(node->args[1], "numeric argument required", 2);
        exit_code = 2;
    }
    else if (node->args[2])
    {
        print_error("exit", "too many arguments", 1);
        return (1);
    }
    else
        exit_code = ft_atoi(node->args[1]) % 256;

    if (!shell->in_pipe)
        cleanup_shell(shell);
    exit(exit_code);
}