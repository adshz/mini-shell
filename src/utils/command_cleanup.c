/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:50:10 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 21:52:09 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"

static void	cleanup_ast_node(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->left)
	{
		cleanup_ast_node(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		cleanup_ast_node(node->right);
		node->right = NULL;
	}
	cleanup_ast_node_data(node);
}

void	cleanup_heredoc_signal_interrupt(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_HEREDOC && g_signal_status == SIG_HEREDOC_INT)
	{
		if (node->data.delimiter)
		{
			free((char *)node->data.delimiter);
			node->data.delimiter = NULL;
		}
	}
	if (node->left)
		cleanup_heredoc_signal_interrupt(node->left);
	if (node->right)
		cleanup_heredoc_signal_interrupt(node->right);
}

void	cleanup_current_command(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->ast)
	{
		cleanup_ast_node(shell->ast);
		shell->ast = NULL;
	}
}

void	cleanup_env_and_cmds(t_shell *shell)
{
	if (shell->cmds)
	{
		ft_lstclear(&shell->cmds, &free_cmd);
		shell->cmds = NULL;
	}
	if (shell->env)
	{
		hashmap_destroy(shell->env);
		shell->env = NULL;
	}
}
