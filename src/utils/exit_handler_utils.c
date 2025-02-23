/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:18:58 by szhong            #+#    #+#             */
/*   Updated: 2025/02/22 11:38:27 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"
#include "errors.h"

void	error_print2(int err)
{
	if (err == CMD_NOT_FOUND)
		ft_putstr_fd(": command not found", STDERR_FILENO);
	else if (err == IS_DIR)
		ft_putstr_fd(": is a directory", STDERR_FILENO);
	else if (err == HIS_WRONG_ARG)
		ft_putstr_fd(": numeric argument required", STDERR_FILENO);
	else if (err == HIS_WRONG_ARGS)
		ft_putstr_fd(": too many arguments", STDERR_FILENO);
	else if (err == NO_FILE || err == CD_ERR)
		ft_putstr_fd(": No such file or directory", STDERR_FILENO);
	else if (err == NOT_DIR)
		ft_putstr_fd(": Not a directory", STDERR_FILENO);
}

void	error_print(char *msg, int err)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (err == INV_ARGS)
		ft_putstr_fd("No arguments needed", STDERR_FILENO);
	else if (err == FORK_ERR)
		ft_putstr_fd("Fork error", STDERR_FILENO);
	else if (err == UNQUOTE)
		ft_putstr_fd("Unmatch quote", STDERR_FILENO);
	else if (err == NO_PERM)
		ft_putstr_fd("Permission denied", STDERR_FILENO);
	else if (err == PIPE_ERR)
		ft_putstr_fd("syntax error near unexpected token", STDERR_FILENO);
	else if (err == SYNTAX_ERR)
		ft_putstr_fd("syntax error near unexpected token", STDERR_FILENO);
	else if (err == HASH_FULL)
		ft_putstr_fd("Insert error: Hash Table is full", STDERR_FILENO);
	else if (err == NO_MEM)
		ft_putstr_fd("No memory left on device: ", STDERR_FILENO);
	else if (err == HIS_WRONG_ARG || err == HIS_WRONG_ARGS)
		ft_putstr_fd("history: ", STDERR_FILENO);
	if (msg)
		ft_putstr_fd(msg, STDERR_FILENO);
	error_print2(err);
	ft_putstr_fd("\n", STDERR_FILENO);
}
