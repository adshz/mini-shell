/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:52:37 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:52:37 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor.h"
#include "errors.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

static int	handle_input_redirection(t_ast_node *node)
{
	int	fd;

	fd = open(node->right->value, O_RDONLY);
	if (fd == -1)
		return (print_error(node->right->value, "No such file or directory", 1));
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

static int	handle_output_redirection(t_ast_node *node)
{
	int	fd;

	fd = open(node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (print_error(node->right->value, "Cannot create file", 1));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

static int	handle_append_redirection(t_ast_node *node)
{
	int	fd;

	fd = open(node->right->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
		return (print_error(node->right->value, "Cannot create file", 1));
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

void	handle_redirections(t_shell *shell, t_ast_node *node)
{
	int	status;

	if (!node)
		return;

	if (node->type == AST_REDIR_IN)
	{
		status = handle_input_redirection(node);
		if (status != 0)
			exit(status);
	}
	else if (node->type == AST_REDIR_OUT)
	{
		status = handle_output_redirection(node);
		if (status != 0)
			exit(status);
	}
	else if (node->type == AST_REDIR_APPEND)
	{
		status = handle_append_redirection(node);
		if (status != 0)
			exit(status);
	}
	else if (node->type == AST_HEREDOC)
	{
		// TODO: Implement heredoc handling
		(void)shell;
	}
}

int	execute_redirection(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	int		status;

	shell->signint_child = true;
	pid = fork();
	if (pid == -1)
		return (print_error(NULL, "fork failed", 1));

	if (pid == 0)
	{
		handle_redirections(shell, node);
		exit(execute_ast(shell, node->left));
	}

	waitpid(pid, &status, 0);
	shell->signint_child = false;
	return (WEXITSTATUS(status));
}


