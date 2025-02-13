/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_executor.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:21:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 17:00:05 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	handle_signal_interrupt(t_shell *shell)
{
	if (g_signal_status == SIGINT)
	{
		shell->signint_child = false;
		shell->in_heredoc = false;
		shell->exit_status = 130;
		g_signal_status = SIG_NONE;
		return (130);
	}
	return (0);
}

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

int	execute_child_process(t_shell *shell, t_ast_node *node)
{
	t_ast_node	*cmd_node;

	setup_redirections(shell, node);
	if (g_signal_status == SIGINT || shell->exit_status == 130)
	{
		g_signal_status = SIG_NONE;
		shell->signint_child = false;
		exit(130);
	}
	cmd_node = find_command_node(node);
	if (!cmd_node)
	{
		exit(shell->exit_status);
	}
	exit(execute_ast(shell, cmd_node));
}

int	handle_redirection_parent_process(t_shell *shell, int status)
{
	if (WIFEXITED(status))
	{
		if (g_signal_status == SIG_HEREDOC_INT)
		{
			shell->signal = 0;
			shell->heredoc_sigint = false;
			g_signal_status = SIG_NONE;
			shell->exit_status = 130;
			return (130);
		}
		return ((status >> 8) & 0xff);
	}
	return (1);
}

int	execute_redirection(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;
	int		ret;

	shell->signint_child = true;
	ret = handle_signal_interrupt(shell);
	if (ret != 0)
		return (ret);
	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));
	if (pid == 0)
		execute_child_process(shell, node);
	waitpid(pid, &status, 0);
	return (handle_redirection_parent_process(shell, status));
}

int	execute_heredoc(t_ast_node *node, t_shell *shell)
{
	if (!node || !node->right)
		return (1);
	return (handle_heredoc(node, shell));
}
