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
#include "executor.h"
#include <readline/readline.h>
#include <readline/history.h>

static void	valid_args(int argc, char **argv)
{
	if (argc > 1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		exit(127);
	}
}

static void	process_line(t_shell *shell)
{
	if (shell->line[0])
		add_history(shell->line);
	shell->tokens = tokenise(shell->line);
	if (!shell->tokens)
	{
		free(shell->line);
		return ;
	}
	shell->ast = parse(shell->tokens);
	if (!shell->ast)
	{
		free(shell->line);
		free_tokens(shell->tokens);
		return ;
	}
}

static void	cleanup_iteration(t_shell *shell)
{
	free_tokens(shell->tokens);
	free_ast(shell->ast);
	free(shell->line);
	shell->tokens = NULL;
	shell->ast = NULL;
	shell->line = NULL;
}

static void	process_command(t_shell *shell)
{
	process_line(shell);
	if (shell->ast && shell->tokens)
	{
		shell->exit_status = execute_ast(shell, shell->ast);
		cleanup_iteration(shell);
	}
}

int	main(int argc, char *argv[], char **envp)
{
	t_shell	shell;

	valid_args(argc, argv);
	init_shell(&shell, argv, envp);
	while (1)
	{
		init_signals();
		shell.line = readline(PROMPT);
		if (!shell.line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		process_command(&shell);
	}
	rl_clear_history();
	cleanup_shell(&shell);
	return (shell.exit_status);
}