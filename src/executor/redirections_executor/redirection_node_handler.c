/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_node_handler.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 00:48:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 00:48:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	collect_redirection_nodes(t_ast_node *node, t_ast_node *redir_nodes[], \
							int *redir_count)
{
	t_ast_node	*current;

	current = node;
	*redir_count = 0;
	while (current && *redir_count < 100)
	{
		if (current->type == AST_REDIR_IN || current->type == AST_REDIR_OUT || \
			current->type == AST_REDIR_APPEND || current->type == AST_HEREDOC)
		{
			redir_nodes[*redir_count] = current;
			(*redir_count)++;
		}
		current = current->left;
	}
	return (0);
}

int	handle_redirection_type(t_ast_node *current, t_shell *shell, \
	int saved_stdin, int saved_stdout)
{
	int	status;

	status = 0;
	if (current->type == AST_REDIR_OUT)
		status = handle_output_redirection(current, shell);
	else if (current->type == AST_REDIR_APPEND)
		status = handle_append_redirection(current, shell);
	else if (current->type == AST_REDIR_IN)
		status = handle_input_redirection(current, shell);
	else if (current->type == AST_HEREDOC)
		status = handle_heredoc(current, shell);
	if (status != 0)
		cleanup_and_exit(saved_stdin, saved_stdout, status);
	return (status);
}
