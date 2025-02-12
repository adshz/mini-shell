/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_interface.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:24:00 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 18:25:03 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_INTERFACE_H
# define SHELL_INTERFACE_H

# include "shell.h"
# include <stdbool.h>

/* Shell operation modes */
bool	nonint_shell(t_shell *shell, const char *input);
void	interactive_loop(t_shell *shell);

/* Shell initialization and cleanup */
int		init_shell(t_shell *shell, char *argv[], char *envp[]);
void	cleanup_shell(t_shell *shell);

#endif 