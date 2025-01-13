/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:22:28 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:25:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include "shell.h"
#include "errors.h"
#include "libft.h"
#include "env_utils.h"
#include "hashtable.h"

static void	get_shell_pid(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit(ERROR);
	if (pid == 0)
		exit(SUCCESS);
	waitpid(pid, NULL, 0);
	shell->pid = pid - 1;
}

void	init_shell(t_shell *shell, char *argv[], char *envp[])
{
	shell->argv = argv;
	shell->envp = envp;
	shell->line = NULL;
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->env = init_env(envp);
	shell->local_vars = init_hashmap();
	shell->alias = init_hashmap();
	shell->cmds = NULL;
	get_shell_pid(shell);
	shell->pids = NULL;
	shell->pid_count = 0;
	shell->old_pwd = NULL;
	shell->history = NULL;
	shell->history_size = 0;
	shell->history_capacity = 0;
	shell->exit_status = 0;
	shell->stdin_backup = -1;
	shell->stdout_backup = -1;
	shell->heredoc_sigint = false;
	shell->signint_child = false;
	shell->in_pipe = false;
	shell->in_heredoc = false;
	
	// Initialize environment variables
	init_env_vars(shell, argv);
}
