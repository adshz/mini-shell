/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:19:36 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:20:45 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "builtins/builtins.h"

void	handle_child_error(const char *cmd)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": Command not found", STDERR_FILENO);
	exit(127);
}

int	check_command(t_shell *shell, t_ast_node *cmd)
{
	if (!cmd || !cmd->value)
		return (0);
	if (is_builtin(cmd->value))
		return (1);
	if (get_command_path(shell, cmd->value, shell->env))
		return (1);
	return (0);
}
