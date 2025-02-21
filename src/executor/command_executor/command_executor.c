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

static void	handle_exit_builtin(t_shell *shell, int exit_status)
{
	if (shell)
	{
		cleanup_current_command(shell);
		cleanup_env_cache(shell);
	}
	exit(exit_status);
}

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
		if (ft_strcmp(node->args[0], "exit") == 0 && \
			!shell->in_pipe && !shell->in_heredoc)
			handle_exit_builtin(shell, ret);
		return (ret);
	}
	ret = execute_external_command(shell, node);
	return (ret);
}

static int	command_executor_execute_child_process(t_shell *shell, \
												t_ast_node *node, \
												char *cmd_path, \
												char **env_array)
{
	if (!cmd_path || !node || !node->args || !env_array)
	{
		if (cmd_path)
			free(cmd_path);
		if (env_array)
			ft_free_array(env_array);
		shell->exit_status = 127;
		return (shell->exit_status);
	}
	setup_child_process();
	if (execve(cmd_path, node->args, env_array) == -1)
	{
		perror("minishell: execve");
		shell->exit_status = 127;
		return (shell->exit_status);
	}
	return (0);
}

static int	handle_external_parent_process(pid_t pid)
{
	int	status;

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
	char	**env_array;
	int		ret;

	if (!node || !node->args || !node->args[0])
		return (1);
	cmd_path = get_command_path(shell, node->args[0], shell->env);
	if (!cmd_path)
	{
		ft_putstr_fd("ERROR: Command path not found\n", 2);
		return (127);
	}
	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		perror("minishell: get_env_array");
		return (1);
	}
	pid = fork();
	if (pid == -1)
	{
		free(cmd_path);
		ft_free_array(env_array);
		return (1);
	}
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		ret = command_executor_execute_child_process(shell, node, \
											cmd_path, env_array);
		free(cmd_path);
		ft_free_array(env_array);
		exit(ret);
	}
	free(cmd_path);
	ft_free_array(env_array);
	return (handle_external_parent_process(pid));
}
