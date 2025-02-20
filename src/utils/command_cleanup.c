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
		return;
	
	// Clean up left subtree
	if (node->left)
	{
		cleanup_ast_node(node->left);
		node->left = NULL;
	}
	
	// Clean up right subtree
	if (node->right)
	{
		cleanup_ast_node(node->right);
		node->right = NULL;
	}
	
	// Clean up node contents
	if (node->args)
	{
		ft_free_array(node->args);
		node->args = NULL;
	}
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	
	// Clean up node itself
	free(node);
}

void	cleanup_current_command(t_shell *shell)
{
	if (!shell)
		return;
	
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
