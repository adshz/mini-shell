/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:58:37 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "errors.h"
#include "libft.h"

int	print_error(char *cmd, char *msg, int error_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (cmd)
	{
		ft_putstr_fd(cmd, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (msg)
		ft_putendl_fd(msg, STDERR_FILENO);
	return (error_code);
}

static const char	*get_error_message(t_error_type error)
{
	static const char	*messages[] = {
	[ERROR_NONE] = "Success",
	[ERROR_MALLOC] = MSG_MALLOC,
	[ERROR_SYNTAX] = MSG_SYNTAX,
	[ERROR_CMD_NOT_FOUND] = MSG_CMD_NOT_FOUND,
	[ERROR_PERMISSION] = MSG_PERMISSION,
	[ERROR_FILE_NOT_FOUND] = MSG_NO_SUCH_FILE,
	[ERROR_PIPE] = "Pipe error",
	[ERROR_FORK] = "Fork error",
	[ERROR_DUP] = "Dup error",
	[ERROR_EXEC] = "Execution error"
	};

	return (messages[error]);
}

static void	handle_error_helper(t_error_type error, const char *detail)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (detail)
	{
		ft_putstr_fd((char *)detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd((char *)get_error_message(error), STDERR_FILENO);
}

void	handle_error(t_shell *shell, t_error_type error, const char *detail)
{
	int	status;

	status = 1;
	if (error == ERROR_CMD_NOT_FOUND)
		status = ERR_CMD_NOT_FOUND;
	else if (error == ERROR_PERMISSION)
		status = ERR_PERMISSION;
	shell->exit_status = status;
	handle_error_helper(error, detail);
}

int	set_error_status(t_shell *shell, int status)
{
	shell->exit_status = status;
	return (SHELL_ERROR);
}
