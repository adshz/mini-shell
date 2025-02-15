/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_setup.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:31:03 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:31:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

void	cleanup_and_exit(int saved_stdin, int saved_stdout, int status)
{
	if (saved_stdin != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
		close(saved_stdin);
	}
	if (saved_stdout != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
		close(saved_stdout);
	}
	exit(status);
}

int	setup_saved_fds(int *saved_stdin, int *saved_stdout)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		cleanup_and_exit(*saved_stdin, *saved_stdout, 1);
		return (1);
	}
	return (0);
}

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

void	setup_redirections(t_shell *shell, t_ast_node *node)
{
	t_ast_node	*redir_nodes[100];
	int			saved_stdin;
	int			saved_stdout;
	int			redir_count;
	int			i;

	if (!node)
		return ;
	saved_stdin = -1;
	saved_stdout = -1;
	if (setup_saved_fds(&saved_stdin, &saved_stdout))
		return ;
	collect_redirection_nodes(node, redir_nodes, &redir_count);

	// First pass: Process all heredocs
	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type == AST_HEREDOC)
		{
			if (collect_heredoc_content(redir_nodes[i], shell) != 0)
			{
				cleanup_and_exit(saved_stdin, saved_stdout, 1);
				return ;
			}
		}
		i--;
	}

	// Second pass: Set up pipes and handle other redirections
	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type != AST_HEREDOC)
			handle_redirection_type(redir_nodes[i], shell, saved_stdin, saved_stdout);
		i--;
	}

	// Final pass: Set up heredoc pipes
	i = redir_count - 1;
	while (i >= 0)
	{
		if (redir_nodes[i]->type == AST_HEREDOC)
			setup_heredoc_pipe(redir_nodes[i]);
		i--;
	}

	shell->exit_status = 0;
}
