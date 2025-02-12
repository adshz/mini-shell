/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_process_handler.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:04:41 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:05:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	execute_command_with_path(t_shell *shell, t_ast_node *node, \
	char *cmd_path)
{
	char	**env_array;
	pid_t	pid;

	env_array = create_env_array(shell->env);
	if (!env_array)
	{
		free(cmd_path);
		shell->exit_status = ERR_MALLOC;
		return (print_error(NULL, MSG_MALLOC, ERR_MALLOC));
	}
	shell->signint_child = true;
	pid = fork();
	if (pid == 0)
		return (execute_command_child(node, cmd_path, env_array));
	return (handle_parent_process(shell, pid, cmd_path, env_array));
}


int	handle_parent_process(t_shell *shell, pid_t pid, \
	char *cmd_path, char **env_array)
{
	int	status;
	int	exit_status;

	if (pid < 0)
	{
		free(cmd_path);
		ft_free_array(env_array);
		shell->exit_status = ERROR_FORK;
		return (print_error(NULL, MSG_FORK, ERROR_FORK));
	}
	waitpid(pid, &status, 0);
	shell->signint_child = false;
	free(cmd_path);
	ft_free_array(env_array);
	if (was_signaled(status))
	{
		exit_status = get_signal_from_status(status);
		shell->exit_status = exit_status;
		return (exit_status);
	}
	return (1);
}

int	execute_command_child(t_ast_node *node, char *cmd_path, char **env_array)
{
	execve(cmd_path, node->args, env_array);
	print_error(cmd_path, strerror(errno), ERR_NOT_EXECUTABLE);
	exit(ERR_NOT_EXECUTABLE);
}
