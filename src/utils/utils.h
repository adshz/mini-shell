/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:24:04 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/26 23:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H
# include <stdio.h>
# include "types.h"
# include "errors.h"
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <termios.h>
# include "memory_collector/memory_collector.h"
# include "parser/includes/ast_manager.h"

void	cleanup_minishell(t_shell *shell);
void	exit_handler(int err);
bool	is_only_whitespace(const char *str);

#endif
