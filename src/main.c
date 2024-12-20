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

static bool	parse_and_build_ast(t_shell *shell)
{
	size_t	i;

	i = 0;
	while (shell->line[i] && (shell->line[i] == ' ' || shell->line[i] == '\t'))
		i++;
	if (!shell->line[i])
		return (false);
	
	if (shell->line[i])
		add_history(shell->line);
	shell->tokens = tokenise(shell->line);
	if (!shell->tokens)
	{
		free(shell->line);
		return (false);
	}
	
	shell->ast = parse(shell->tokens);
	if (!shell->ast)
	{
		free(shell->line);
		free_tokens(shell->tokens);
		return (false);
	}

	return (true);
}

static void	interactive_loop(t_shell *shell)
{
	while (1)
	{
		init_signals();
		shell->line = readline(PROMPT);
		if (!shell->line)
		{
			ft_putendl_fd("exit", STDOUT_FILENO);
			break;
		}
		if (shell->line[0] == '\0')
		{
			free(shell->line);
			continue;
		}
		if (parse_and_build_ast(shell))
		{
			if (shell->ast)
			{
				shell->exit_status = execute_ast(shell, shell->ast);
				add_command_to_history(shell, shell->ast);
				shell->ast = NULL;
			}
			
			if (ft_strcmp(shell->line, "exit") == 0)
			{
				cleanup_current_command(shell);
				break;
			}
			cleanup_current_command(shell);
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