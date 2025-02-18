/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 21:48:34 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 21:48:55 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "types.h"
#include "builtins/builtins.h"
#include "core/core.h"

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

static int	execute_child_process(t_shell *shell, t_ast_node *node, \
								char *cmd_path)
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

static int	handle_external_parent_process(pid_t pid, char *cmd_path)
{
	int	status;

	free(cmd_path);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	restore_signal_handlers();
	if (was_signaled(status))
	{
		if (get_signal_from_status(status) == SIGQUIT)
		{
			ft_putendl_fd("Quit (core dumped)", STDERR_FILENO);
			return (131);
		}
		if (get_signal_from_status(status) == SIGINT)
			return (130);
	}
	return (get_exit_status(status));
}

int	execute_external_command(t_shell *shell, t_ast_node *node)
{
	pid_t	pid;
	char	*cmd_path;

	if (!node || !node->args || !node->args[0])
		return (1);
	cmd_path = get_command_path(shell, node->args[0], shell->env);
	if (!cmd_path)
		return (127);
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		execute_child_process(shell, node, cmd_path);
	}
	return (handle_external_parent_process(pid, cmd_path));
}
