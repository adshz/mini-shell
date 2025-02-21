/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:01:56 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:03:28 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "utils/utils.h"

int	handle_cd_redirection(t_shell *shell, t_ast_node *node)
{
	int	saved_stdout;
	int	ret;

	saved_stdout = dup(STDOUT_FILENO);
	if (saved_stdout == -1)
		return (print_error(NULL, "dup failed", 1));
	setup_redirections(shell, node);
	ret = execute_builtin(shell, node->left);
	if (dup2(saved_stdout, STDOUT_FILENO) == -1)
		ret = print_error(NULL, "dup2 failed", 1);
	close(saved_stdout);
	shell->exit_status = ret;
	return (ret);
}

int	handle_other_builtin_redirection(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;
	int		ret;

	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));
	if (pid == 0)
	{
		setup_redirections(shell, node);
		ret = execute_builtin(shell, node->left);
		exit(ret);
	}
	waitpid(pid, &status, 0);
	ret = (status >> 8) & 0xff;
	shell->exit_status = ret;
	if (node->left->args && ft_strcmp(node->left->args[0], "exit") == 0)
		exit(ret);
	return (ret);
}

int	handle_builtin_redirection(t_shell *shell, t_ast_node *node)
{
	if (ft_strcmp(node->left->args[0], "cd") == 0)
		return (handle_cd_redirection(shell, node));
	return (handle_other_builtin_redirection(shell, node));
}

int	handle_redirection_node(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (node->left && node->left->args && is_builtin(node->left->args[0]))
		return (handle_builtin_redirection(shell, node));
	ret = execute_redirection(shell, node);
	shell->exit_status = ret;
	return (ret);
}
