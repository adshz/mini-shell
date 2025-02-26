/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signals.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:56:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 20:55:36 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef FT_SIGNALS_H
# define FT_SIGNALS_H

# define SHELL_STATE_READLINE 1
# define SHELL_STATE_EXECUTION 2
# define SHELL_STATE_HEREDOC_INPUT 42
# define SHELL_STATE_HEREDOC_INTERRUPTED 43
# define SHELL_STATE_NORMAL 0

void	config_main_signals(t_shell *shell);
void	config_execution_signals(void)
#endif
