/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:33:01 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 09:20:20 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "types.h"
#include "builtins/builtins.h"

int	execute_command(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (!node || !node->args || !node->args[0])
		return (1);
	
	ret = validate_and_expand_command(shell, node);
	if (ret != 0)
		return (ret);

	if (is_builtin(node->args[0]))
	{
		ret = execute_builtin(shell, node);
		if (ft_strcmp(node->args[0], "exit") == 0)
			if (!shell->in_heredoc)
				exit(ret);
		return (ret);
	}

	ret = execute_external_command(shell, node);
	return (ret);
}

static int	execute_child_process(t_shell *shell,
		t_ast_node *node, char *cmd_path)
{
	char	**env_array;

	setup_child_process();
	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		exit(1);
	}
	execve(cmd_path, node->args, env_array);
	ft_free_array(env_array);
	free(cmd_path);
	exit(127);
}

static int	handle_external_parent_process(t_shell *shell,
		pid_t pid, char *cmd_path)
{
	int	status;

	setup_parent_process();
	waitpid(pid, &status, 0);
	free(cmd_path);
	init_signals();
	if (was_signaled(status))
		return (handle_signal_termination(shell, status));
	shell->exit_status = get_exit_status(status);
	return (get_exit_status(status));
}

int	execute_external_command(t_shell *shell, t_ast_node *node)
{
	char	*cmd_path;
	int		ret;
	pid_t	pid;

	ret = handle_path_resolution(shell, node, &cmd_path);
	if (ret != 0)
		return (ret);
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
		return (execute_child_process(shell, node, cmd_path));
	return (handle_external_parent_process(shell, pid, cmd_path));
}
