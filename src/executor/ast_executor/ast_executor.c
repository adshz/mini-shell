/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:00:39 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:59:08 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	execute_ast(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (!node)
		return (0);
	if (shell->heredoc_sigint)
	{
		cleanup_current_command(shell);
		return (130);
	}
	ret = handle_node_by_type(shell, node);
	if (ret != -1)
		return (ret);
	if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT || \
		node->type == AST_REDIR_APPEND || node->type == AST_HEREDOC)
		return (handle_redirection_node(shell, node));
	shell->exit_status = 1;
	return (1);
}
