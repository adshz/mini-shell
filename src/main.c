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
#include "shell.h"

static void	valid_args(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILNO);
		ft_putstr_fd(argv[1], STDERR_FILNO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		exit(127);
	}
}

int	main(int agrc, char *argv[], char **envp)
{
	t_minishell	shell;

	valid_args(argc, argv);
	init_shell(&shell, envp);
	while (1)
	{
		init_signals();
		shell.line = readline(PROMPT);
		if (!shell.line)
			break ;
		if (shell.line[0])
			add_history(shell.line);
		shell.tokens = tokenise(shell.line);
		if (!shell.tokens)
			continue ;
		shell.ast = parse(shell.tokens);
		if (!shell.ast)
		{
			free_tokens(shell_tokens);
			continue ;
		}
		shell.exit_status = execute_ast(shell.ast);
		free_tokens(shell.tokens);
		free_ast(shell.ast);
		free(shell.line);
	}
	cleanup_shell(&shell);
	return (shell.exit_status);
}
