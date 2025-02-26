/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_shell.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 23:49:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 23:50:30 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils/utils.h"

void	cleanup_minishell(t_shell *shell)
{
	ft_memory_collector(shell, NULL, true);
	ft_cleanup_ast(shell, &shell->ast);
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_termset);
}
