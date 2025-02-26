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
			continue ;
		if (shell->ast)
		{
			shell_start_execution(shell); // remember to assign value to shell->exit_status 
			add_command_to_history(shell, shell->ast);
		}
		ft_cleanup_ast(shell, &shell->ast);
	}
	exit_handler(shell, NULL, SHELL_SUCCESS, IS_EXIT)
}
