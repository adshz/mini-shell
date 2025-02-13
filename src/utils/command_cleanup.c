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

void	cleanup_current_command(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->ast && !shell->cmds)
	{
		free_ast(shell->ast);
		shell->ast = NULL;
	}
	if (shell->tokens)
	{
		free_tokens(shell->tokens);
		shell->tokens = NULL;
	}
	if (shell->line)
	{
		free(shell->line);
		shell->line = NULL;
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
