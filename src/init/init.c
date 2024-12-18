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
#include "shell.h"
#include "errors.h"
#include "libft.h"

static void		get_shell_pid(t_shell *shell)
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

static	void	check_env(t_shell *shell, char *argv[])
{
	char	*tmp;
	int		shlvl;

	if (!hashmap_search(shell->env, "PWD"))
	{
		tmp = getcwd(NULL, 0);
		hashmap_insert(shell->env, "PWD", tmp, 0);
		free(tmp);
	}
	shlvl = 1;
	tmp = hashmap_search(shell->env, "SHLVL");
	if (tmp && ft_atoi(tmp) > 0)
		shlvl = ft_atoi(tmp) + 1;
	tmp = ft_itoa(shlvl);
	hashmap_insert(shell->env, "SHLVL", tmp, 0);
	free(tmp);
	if (!hashmap_search(shell->env, "PATH"))
		hashmap_insert(shell->env, "PATH",\
				 "/usr/local/sbin:/usr/local/bin:/usr/bin:/bin", 0);
	if (!hashmap_search(shell->env, "_"))
		hashmap_insert(shell->env, "_", argv[0], 0);
}

void	init_shell(t_shell *shell, char *argv[], char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->exit_status = 0;
	shell->cmds = NULL;
	
	// Create empty environment if envp is NULL
	if (!envp)
	{
		shell->env = hashmap_create();
		if (shell->env)
		{
			hashmap_insert(shell->env, "PATH", "/usr/local/bin:/usr/bin:/bin", 0);
			hashmap_insert(shell->env, "SHLVL", "1", 0);
			// Add any other essential environment variables
		}
	}
	else
		shell->env = env_to_hash(envp);
	
	shell->pids = NULL;
	shell->old_pwd = NULL;
	shell->history = NULL;
	shell->stdin_backup = dup(STDIN_FILENO);
	shell->stdout_backup = dup(STDOUT_FILENO);
	tcgetattr(STDIN_FILENO, &shell->term_settings);
	check_env(shell, argv);
	get_shell_pid(shell);
}
