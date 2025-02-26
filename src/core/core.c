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

/**
 * @brief Validates user input for shell processing
 *
 * Checks for:
 * - NULL input (EOF/Ctrl+D): Prints "exit" and returns false
 * - Empty line: Frees line and returns true so we continue the loop
 *
 * @param shell Pointer to shell structure
 * @return true if input is valid for processing, false if shell should exit
 *         or skip processing
 *
 * @note readline() returns NULL on Ctrl+D (EOF) or error
 */
static bool	valid_usr_input(t_shell *shell)
{
	if (!shell->line)
	{
		ft_putendl_fd("exit", STDOUT_FILENO);
		return (false);
	}
	if (shell->line[0] == '\0' || is_only_whitespace(shell->line))
	{
		free(shell->line);
		shell->line = NULL;
		return (true);
	}
	return (true);
}

/**
 * @brief Parses tokenised line and builds AST
 *
 * Process:
 * 1. Validates input line
 * 2. Adds command to history
 * 3. Builds AST from input
 *
 * @param shell Pointer to shell structure
 * @return true if parsing succeeds, false otherwise
 *
 * @note Handles memory cleanup on failure
 */
static bool	parse_and_build_ast(t_shell *shell)
{
	if (!shell->line || shell->line[0] == '\0')
	{
		free(shell->line);
		shell->line = NULL;
		return (false);
	}
	add_history(shell->line);
	if (!build_ast_tree(shell))
		return (false);
	return (true);
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
		if (!valid_usr_input(shell))
			break ;
		if (!parse_and_build_ast(shell))
		{
			if (shell->parse_err.type)
				parse_err_handler(shell);
			continue ;
		}
		shell_start_execution(shell);
		ft_cleanup_ast(shell, &shell->ast);
	}
	exit_handler(shell, NULL, SHELL_SUCCESS, IS_EXIT)
}
