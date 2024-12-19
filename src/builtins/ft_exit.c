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

static void	cleanup_shell(t_shell *shell)
{
	if (shell->env)
		hashmap_destroy(shell->env);
	if (shell->cmds)
		free_ast(shell->cmds);
	if (shell->pids)
		free(shell->pids);
	if (shell->old_pwd)
		free(shell->old_pwd);
	if (shell->history)
		free_history(shell->history);
	if (shell->stdin_backup > 2)
		close(shell->stdin_backup);
	if (shell->stdout_backup > 2)
		close(shell->stdout_backup);
}

int	builtin_exit(t_shell *shell, t_ast_node *node)
{
	int	exit_code;

	ft_putendl_fd("exit", STDERR_FILENO);
	if (!node->args || !node->args[1])
	{
		exit_code = shell->exit_status;
		cleanup_shell(shell);
		exit(exit_code);
	}
	if (!ft_isnumber(node->args[1]))
	{
		print_error(node->args[1], "numeric argument required", 2);
		cleanup_shell(shell);
		exit(2);
	}
	if (node->args[2])
	{
		print_error("exit", "too many arguments", 1);
		return (1);
	}
	exit_code = ft_atoi(node->args[1]) % 256;
	cleanup_shell(shell);
	exit(exit_code);
}


