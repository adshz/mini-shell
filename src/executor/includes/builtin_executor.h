/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_executor.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:32:38 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:33:01 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef BUILTIN_EXECUTOR_H
# define BUILTIN_EXECUTOR_H
# include "../executor.h"

int	execute_builtin(t_shell *shell, char **argv);
bool	is_builtin(const char *cmd);
#endif
