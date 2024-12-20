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
	
	ft_putstr_fd("Debug: Processing command: '", 2);
	ft_putstr_fd(shell->line, 2);
	ft_putstr_fd("'\n", 2);
	
	if (shell->line[i])
		add_history(shell->line);
	shell->tokens = tokenise(shell->line);
	if (!shell->tokens)
	{
		ft_putstr_fd("Debug: Tokenization failed\n", 2);
		free(shell->line);
		return (false);
	}
	
	// Debug print tokens
	ft_putstr_fd("Debug: Tokens:\n", 2);
	t_token *current = shell->tokens;
	while (current)
	{
		ft_putstr_fd("Token: type=", 2);
		ft_putnbr_fd(current->type, 2);
		ft_putstr_fd(", value='", 2);
		ft_putstr_fd(current->value, 2);
		ft_putstr_fd("'\n", 2);
		current = current->next;
	}
	
	shell->ast = parse(shell->tokens);
	if (!shell->ast)
	{
		ft_putstr_fd("Debug: AST creation failed\n", 2);
		free(shell->line);
		free_tokens(shell->tokens);
		return (false);
	}

	ft_putstr_fd("Debug: AST created successfully\n", 2);
	
	// Debug print AST node
	ft_putstr_fd("Debug: AST node type: ", 2);
	ft_putnbr_fd(shell->ast->type, 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd("Debug: AST node value: '", 2);
	if (shell->ast->value)
		ft_putstr_fd(shell->ast->value, 2);
	ft_putstr_fd("'\n", 2);
	
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
		// Skip empty lines
		if (shell->line[0] == '\0')
		{
			free(shell->line);
			continue;
		}
		if (parse_and_build_ast(shell))
		{
			ft_putstr_fd("Debug: About to execute AST\n", 2);
			if (shell->ast)
			{
				shell->exit_status = execute_ast(shell, shell->ast);
				add_command_to_history(shell, shell->ast);
				shell->ast = NULL;
			}
			else
			{
				ft_putstr_fd("Debug: AST is NULL before execution\n", 2);
			}
			
			// If exit was called, we should break the loop
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