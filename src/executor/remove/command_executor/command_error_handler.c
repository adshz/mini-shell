/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_error_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:34:13 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 14:47:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	handle_command_not_found(t_shell *shell, const char *cmd)
{
	shell->exit_status = 127;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (127);
}

int	validate_and_expand_command(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (!node->args[0])
		return (1);
	if (expand_command_args(shell, node))
		return (1);
	ret = handle_special_paths(shell, node);
	if (ret != 0)
		return (ret);
	return (0);
}
