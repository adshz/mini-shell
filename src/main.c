/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:03:22 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:22:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <assert.h>

#include "shell.h"
#include "executor.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	validate_args(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		exit(127);
	}
}



static void	interactive_loop(t_shell *shell)
{
	while (1)
	{
		init_signals();
		if (!nonint_shell(shell, readline(PROMPT)))
		{
			break;
		}
	}
	cleanup_shell(shell);
}

int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;
	int		exit_status;

	validate_args(argc, argv);
	init_shell(&shell, argv, envp);
	interactive_loop(&shell);
	exit_status = shell.exit_status;
	rl_clear_history();
	cleanup_shell(&shell);
	return (exit_status);
}