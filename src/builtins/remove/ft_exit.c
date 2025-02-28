/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 17:49:13 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 22:07:12 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include "utils/utils.h"
#include "errors.h"

static int	get_exit_code(t_shell *shell, t_ast_node *node)
{
	int	exit_code;

	if (!node || !node->args)
		exit_code = shell->exit_status;
	else if (!node->args[0])
		exit_code = shell->exit_status;
	else if (node->args[1] && !ft_isnumber(node->args[1]))
	{
		print_error(node->args[1], "numeric argument required", 2);
		exit_code = 2;
	}
	else if (node->args[1] && node->args[2])
	{
		print_error("exit", "too many arguments", 1);
		exit_code = 1;
	}
	else if (node->args[1])
		exit_code = ft_atoi(node->args[1]) % 256;
	else
		exit_code = shell->exit_status;
	return (exit_code);
}

int	builtin_exit(t_shell *shell, t_ast_node *node)
{
	int	exit_code;

	if (!shell || !node)
		return (1);
	exit_code = get_exit_code(shell, node);
	if (!shell->in_pipe)
		ft_putendl_fd("exit", STDERR_FILENO);
	if (exit_code == 1 && node->args && node->args[0] && \
		node->args[1] && node->args[2] && !shell->in_pipe)
		return (1);
	if (shell->in_pipe)
		return (exit_code);
	cleanup_shell(shell);
	exit(exit_code);
}
