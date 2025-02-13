/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_child.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:15:15 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 15:24:30 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"
#include "parser/parser.h"

void	execute_left_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	t_ast_node	*cmd_node;
	int			ret;
	char		*error_msg;

	close(pipe_fd[0]);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		exit(1);
	close(pipe_fd[1]);
	setup_redirections(shell, node->left);
	cmd_node = node->left;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;
	if (cmd_node)
		error_msg = cmd_node->value;
	else
		error_msg = "";
	if (!cmd_node || !check_command(shell, cmd_node))
		handle_child_error(error_msg);
	ret = execute_ast(shell, node->left);
	exit(ret);
}

void	execute_right_child(t_shell *shell, t_ast_node *node, int pipe_fd[2])
{
	t_ast_node	*cmd_node;
	int			ret;
	char		*error_msg;

	close(pipe_fd[1]);
	if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
		exit(1);
	close(pipe_fd[0]);
	setup_redirections(shell, node->right);
	cmd_node = node->right;
	while (cmd_node && cmd_node->type != AST_COMMAND)
		cmd_node = cmd_node->left;
	if (cmd_node)
		error_msg = cmd_node->value;
	else
		error_msg = "";
	if (!cmd_node || !check_command(shell, cmd_node))
		handle_child_error(error_msg);
	ret = execute_ast(shell, node->right);
	exit(ret);
}
