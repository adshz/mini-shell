/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_printer.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:59:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:59:56 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

int	exec_print_err(t_err err)
{
	if (err.msg == MSGERR_CMD_NOT_FOUND)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd(": command not found\n", 2), err.num);
	else if (err.msg == MSGERR_NO_SUCH_FILE)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd(": no such file or directory\n", 2), err.num);
	else if (err.msg == MSGERR_PERM_DENIED)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd(": Permission denied\n", 2), err.num);
	else if (err.msg == MSGERR_AMBIGUOUS)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd(": ambiguous redirect\n", 2), err.num);
	else if (err.msg == MSGERR_TOO_MANY_ARGS)
		return (ft_putstr_fd("minishell: exit: too many argument\n", 2), err.num);
	else if (err.msg == MSGERR_NUMERIC_REQUI)
		return (ft_putstr_fd("minishell: ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd("minishell: exit ", 2), ft_putstr_fd(err.prob, 2), \
		ft_putstr_fd(": numeric argument required\n", 2), err.num);
	return (0);
}
