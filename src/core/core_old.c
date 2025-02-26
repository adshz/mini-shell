/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core_old.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 15:33:21 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:49:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "core.h"
#include "shell.h"
#include <signal.h>

/* --------- cleanup process ---------*/

static void	
void	cleanup_minishell(t_shell *shell)
{
	ft_memory_collector(shell, NULL, true);
	ft_cleanup_ast(shell, &shell->ast);
	rl_clear_history();
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_termset);
}
