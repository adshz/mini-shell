/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:45:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:07:34 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static void	debug_external_cmd(const char *msg, const char *extra)
{
	ft_putstr_fd("DEBUG [external]: ", 2);
	ft_putstr_fd(msg, 2);
	if (extra)
	{
		ft_putstr_fd(": ", 2);
		ft_putstr_fd(extra, 2);
	}
	ft_putstr_fd("\n", 2);
}

static void	debug_error_num(const char *msg, int num)
{
	char	*num_str;
	char	buffer[32];

	ft_putstr_fd("DEBUG [external]: ", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd(": ", 2);
	ft_itoa_buffer(num, buffer, sizeof(buffer));
	ft_putstr_fd(buffer, 2);
	ft_putstr_fd("\n", 2);
}

static void	debug_argv(char **argv)
{
	int	i;

	i = 0;
	ft_putstr_fd("DEBUG [external]: argv contents:\n", 2);
	while (argv && argv[i])
	{
		ft_putstr_fd("  argv[", 2);
		ft_putnbr_fd(i, 2);
		ft_putstr_fd("]: ", 2);
		ft_putstr_fd(argv[i], 2);
		ft_putstr_fd("\n", 2);
		i++;
	}
}

static void	debug_path_resolution(t_shell *shell, const char *cmd)
{
	char	*path_var;

	path_var = hashmap_search(shell->env, "PATH");
	debug_external_cmd("Resolving path for command", cmd);
	debug_external_cmd("PATH environment variable", path_var ? path_var : "NULL");
}

int	execute_external_cmd(t_shell *shell, t_ast_node *node)
{
	t_path	path_status;
	int		tmp_status;
	int		child_pid;

	g_signal_status = SHELL_STATE_EXECUTION;
	debug_external_cmd("Starting execution", NULL);
	child_pid = fork();
	if (child_pid == 0)
	{
		debug_external_cmd("In child process", NULL);
		tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
		debug_external_cmd("Restored terminal settings", NULL);
		
		tmp_status = check_redirection(node);
		if (tmp_status != ERRNO_NONE)
		{
			debug_external_cmd("Redirection check failed", NULL);
			(cleanup_minishell(shell), exit(ERRNO_GENERAL));
		}
		debug_external_cmd("Redirection check passed", NULL);

		debug_path_resolution(shell, (node->expanded_argv)[0]);
		path_status = get_path(shell, (node->expanded_argv)[0]);
		debug_error_num("Path status error number", path_status.err.num);
		if (path_status.err.num != ERRNO_NONE)
		{
			debug_external_cmd("Path resolution failed", path_status.path);
			tmp_status = exec_print_err(path_status.err);
			(cleanup_minishell(shell), exit(tmp_status));
		}
		if (!path_status.path)
		{
			debug_external_cmd("Path is NULL", NULL);
			(cleanup_minishell(shell), exit(1));
		}
		debug_external_cmd("Found command path", path_status.path);
		debug_argv(node->expanded_argv);

		debug_external_cmd("Attempting to execute command", NULL);
		if (execve(path_status.path, node->expanded_argv, shell->environ) == -1)
		{
			debug_external_cmd("execve failed with error", strerror(errno));
			free(path_status.path);
			(cleanup_minishell(shell), exit(1));
		}
	}
	debug_external_cmd("Waiting for child process", NULL);
	waitpid(child_pid, &tmp_status, 0);
	debug_external_cmd("Child process finished", NULL);
	g_signal_status = SHELL_STATE_NORMAL;
	return (exec_get_exit_status(tmp_status));
}
