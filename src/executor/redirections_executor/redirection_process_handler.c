/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_process_handler.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:22:39 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:22:43 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_ast_node	*find_command_node(t_ast_node *node)
{
	t_ast_node	*cmd_node;

	cmd_node = node;
	while (cmd_node && (cmd_node->type == AST_REDIR_IN || \
		cmd_node->type == AST_REDIR_OUT || \
		cmd_node->type == AST_REDIR_APPEND || \
		cmd_node->type == AST_HEREDOC))
	{
		cmd_node = cmd_node->left;
	}
	return (cmd_node);
}

static void	handle_child_signal_interrupt(t_shell *shell)
{
	g_signal_status = SIG_NONE;
	shell->signint_child = false;
	if (!shell->heredoc_sigint)
	{
		cleanup_current_command(shell);
		cleanup_env_cache(shell);
	}
	exit(130);
}

int	execute_child_process(t_shell *shell, t_ast_node *node)
{
	t_ast_node	*cmd_node;
	int			exit_status;

	cmd_node = NULL;
	exit_status = 0;
	setup_redirections(shell, node);
	if (g_signal_status == SIGINT || shell->exit_status == 130)
		handle_child_signal_interrupt(shell);
	cmd_node = find_command_node(node);
	if (!cmd_node)
	{
		exit_status = shell->exit_status;
		cleanup_current_command(shell);
		cleanup_env_cache(shell);
		exit(exit_status);
	}
	exit_status = execute_ast(shell, cmd_node);
	cleanup_current_command(shell);
	cleanup_env_cache(shell);
	exit(exit_status);
}
