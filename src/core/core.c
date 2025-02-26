/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/25 04:58:42 by szhong            #+#    #+#             */
/*   Updated: 2025/02/25 15:56:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/**
 * @brief Signal handler for handling signals
 *
 * This function is used to handle signals in the shell.
 * It sets the signal status to the signal number.
 * 
 * Without extern, each file would create its own separate variable 
 * instead of using the one defined in main.c
 */
volatile sig_atomic_t	g_signal_status = 0;

static bool	is_eof_detected(t_shell *shell)
{
	if (!shell->line)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (true);
	}
	return (false);
}

static book	is_empty_input(t_shell *shell)
{
	if (shell->line[0] == '\0' || is_only_whitespace(shell->line))
	{
		free(shell->line);
		shell->line = NULL;
		return (true);
	}
	return (false);
}

/**
 * @brief Main interactive shell loop
 *
 * Process flow:
 * 1. Initialize signals
 * 2. Read user input
 * 3. Parse and build AST
 * 4. Execute commands
 * 5. Update history
 * 6. Cleanup
 *
 * @param shell Pointer to shell structure
 *
 * @note Continues until EOF or exit command
 */
void	interactive_loop(t_shell *shell)
{
	while (1)
	{
		config_main_signals(shell);
		shell->line = readline(PROMPT);
		if (is_eof_detected(shell))
			break ;
		if (is_empty_input(shell))
			continue ;
		shell->tokens = tokenise(shell, shell->line);
		if (!shell->tokens)
			continue ;
		shell->ast = parse(shell, shell->tokens);
		if (shell->parse_err.type)
		{
			parse_err_handler(shell);
			continue ;
		}
		execute_shell_command(shell);
		ft_cleanup_ast(shell, &shell->ast);
	}
	exit_handler(shell, NULL, SHELL_SUCCESS, IS_EXIT)
}
