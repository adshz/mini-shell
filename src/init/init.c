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

static void	init_shlvl(t_shell *shell)
{
	char	*tmp;
	int		shlvl;

	shlvl = 1;
	tmp = hashmap_search(shell->env, "SHLVL");
	if (tmp && ft_atoi(tmp) > 0)
		shlvl = ft_atoi(tmp) + 1;
	tmp = ft_itoa(shlvl);
	if (!tmp)
		return ;
	hashmap_insert(shell->env, "SHLVL", tmp, 0);
	free(tmp);
}

static void	init_pwd(t_shell *shell)
{
	char	*tmp;

	if (!hashmap_search(shell->env, "PWD"))
	{
		tmp = getcwd(NULL, 0);
		if (!tmp)
			return ;
		hashmap_insert(shell->env, "PWD", tmp, 0);
		free(tmp);
	}
}

static void	init_env_vars(t_shell *shell, char *argv[])
{
	init_pwd(shell);
	init_shlvl(shell);
	if (!hashmap_search(shell->env, "PATH"))
		hashmap_insert(shell->env, "PATH",
			"/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", 0);
	if (!hashmap_search(shell->env, "_"))
		hashmap_insert(shell->env, "_", argv[0], 0);
	hashmap_remove(shell->env, "OLDPWD");
}

void	init_shell(t_shell *shell, char *argv[], char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->exit_status = 0;
	shell->cmds = NULL;
	shell->pids = NULL;
	shell->old_pwd = NULL;
	shell->history = NULL;
	
	if (!envp)
		shell->env = hashmap_create();
	else
		shell->env = env_to_hash(envp);
	
	if (!shell->env)
		exit(ERROR);
	
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	tcgetattr(STDIN_FILENO, &shell->term_settings);
	init_env_vars(shell, argv);
	get_shell_pid(shell);
}
