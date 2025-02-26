/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_processor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 01:15:30 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 01:15:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

static int	collect_heredoc_contents(t_shell *shell, t_ast_node **redir_nodes,
		int redir_count, int *saved_fds)
{
	int	i;

	i = 0;
	while (i < redir_count)
	{
		if (redir_nodes[i]->type == AST_HEREDOC)
		{
			if (collect_heredoc_content(redir_nodes[i], shell) != 0)
			{
				cleanup_and_exit(saved_fds[0], saved_fds[1], 1);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	setup_heredoc_pipes(t_ast_node **redir_nodes,
		int redir_count, int *saved_fds)
{
	int	i;

	i = 0;
	while (i < redir_count)
	{
		if (redir_nodes[i]->type == AST_HEREDOC)
		{
			if (setup_heredoc_pipe(redir_nodes[i]) != 0)
			{
				cleanup_and_exit(saved_fds[0], saved_fds[1], 1);
				return (1);
			}
		}
		i++;
	}
	return (0);
}

static int	process_heredoc_nodes(t_shell *shell, t_ast_node **redir_nodes,
		int redir_count, int *saved_fds)
{
	if (collect_heredoc_contents(shell, redir_nodes, redir_count, \
							saved_fds) != 0)
		return (1);
	if (setup_heredoc_pipes(redir_nodes, redir_count, saved_fds) != 0)
		return (1);
	return (0);
}

static int	process_other_redirections(t_shell *shell, t_ast_node **redir_nodes,
		int redir_count, int *saved_fds)
{
	int	i;

	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type != AST_HEREDOC)
		{
			if (handle_redirection_type(redir_nodes[i], shell,
					saved_fds[0], saved_fds[1]) != 0)
			{
				cleanup_and_exit(saved_fds[0], saved_fds[1], 1);
				return (1);
			}
		}
		i--;
	}
	return (0);
}

void	setup_redirections(t_shell *shell, t_ast_node *node)
{
	t_ast_node	*redir_nodes[100];
	int			saved_fds[2];
	int			redir_count;

	if (!node)
		return ;
	saved_fds[0] = -1;
	saved_fds[1] = -1;
	if (setup_saved_fds(&saved_fds[0], &saved_fds[1]))
		return ;
	if (collect_redirection_nodes(node, redir_nodes, &redir_count) != 0)
	{
		cleanup_and_exit(saved_fds[0], saved_fds[1], 1);
		return ;
	}
	if (process_heredoc_nodes(shell, redir_nodes, redir_count, saved_fds) != 0)
		return ;
	if (process_other_redirections(shell, redir_nodes, \
							redir_count, saved_fds) != 0)
		return ;
	shell->exit_status = 0;
}
