/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:13 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 17:49:16 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins.h"
#include "libft.h"
#include "utils/utils.h"
#include "errors.h"

static int get_exit_code(t_shell *shell, t_ast_node *node)
{
    int exit_code;

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
        exit_code = 1;
    }
    else
        exit_code = ft_atoi(node->args[1]) % 256;
    return (exit_code);
}

int builtin_exit(t_shell *shell, t_ast_node *node)
{
    int exit_code;

    if (!shell || !node)
        return (1);
    if (!shell->in_pipe)
        ft_putendl_fd("exit", STDERR_FILENO);

    exit_code = get_exit_code(shell, node);
    if (exit_code == 1 && node->args && node->args[2])
        return (1);
    exit(exit_code);
}
