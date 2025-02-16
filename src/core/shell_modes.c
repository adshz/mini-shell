/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_modes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 16:16:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 23:36:48 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "shell_interface.h"
#include "shell.h"
#include "libft.h"
#include "utils/utils.h"

// void	interactive_loop(t_shell *shell)
// {
// 	while (1)
// 	{
// 		init_signals();
// 		if (!nonint_shell(shell, readline(PROMPT)))
// 			break ;
// 	}
// 	cleanup_shell(shell);
// }

// bool	nonint_shell(t_shell *shell, const char *input)
// {
// 	bool	success;

// 	if (!input)
// 	{
// 		ft_putendl_fd("exit", STDOUT_FILENO);
// 		return (false);
// 	}
// 	shell->line = ft_strdup(input);
// 	if (!shell->line)
// 		return (false);
// 	if (shell->line[0] == '\0' || is_only_whitespace(shell->line))
// 	{
// 		free(shell->line);
// 		shell->line = NULL;
// 		return (true);
// 	}
// 	success = parse_and_build_ast(shell);
// 	if (success && shell->ast)
// 	{
// 		shell->exit_status = execute_ast(shell, shell->ast);
// 		add_command_to_history(shell, shell->ast);
// 		shell->ast = NULL;
// 	}
// 	free(shell->line);
// 	shell->line = NULL;
// 	return (success);
// }
