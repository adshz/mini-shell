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
	int	i;

	if (!node || !node->args || !node->args[0])
		return (1);
	ft_putstr_fd("\nDEBUG: execute_command called with command: [",
		STDERR_FILENO);
	ft_putstr_fd(node->args[0], STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	ft_putstr_fd("DEBUG: Arguments:\n", STDERR_FILENO);
	i = 1;
	while (node->args[i])
	{
		ft_putstr_fd("  arg[", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd("]: [", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		i++;
	}
	ret = validate_and_expand_command(shell, node);
	if (ret != 0)
	{
		ft_putstr_fd("DEBUG: Command validation/expansion failed\n",
			STDERR_FILENO);
		return (ret);
	}
	ft_putstr_fd("DEBUG: After expansion:\n", STDERR_FILENO);
	i = 0;
	while (node->args[i])
	{
		ft_putstr_fd("  arg[", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd("]: [", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		i++;
	}
	if (is_builtin(node->args[0]))
	{
		ft_putstr_fd("DEBUG: Executing builtin command\n", STDERR_FILENO);
		ret = execute_builtin(shell, node);
		if (ft_strcmp(node->args[0], "exit") == 0)
			if (!shell->in_heredoc)
				exit(ret);
		return (ret);
	}
	ft_putstr_fd("DEBUG: Executing external command\n", STDERR_FILENO);
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

	ft_putstr_fd("\nDEBUG: execute_external_command called\n",
		STDERR_FILENO);
	ret = handle_path_resolution(shell, node, &cmd_path);
	if (ret != 0)
	{
		ft_putstr_fd("DEBUG: Path resolution failed\n", STDERR_FILENO);
		return (ret);
	}
	ft_putstr_fd("DEBUG: Resolved command path: [", STDERR_FILENO);
	ft_putstr_fd(cmd_path, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
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
