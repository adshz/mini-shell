/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <unistd.h>
#include "errors.h"
#include "libft.h"

/* Legacy error handling function */
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
		[ERR_NONE] = "Success",
		[ERR_MALLOC_NEW] = MSG_MALLOC,
		[ERR_SYNTAX_NEW] = MSG_SYNTAX,
		[ERR_CMD_NOT_FOUND_NEW] = MSG_CMD_NOT_FOUND,
		[ERR_PERMISSION_NEW] = MSG_PERMISSION,
		[ERR_FILE_NOT_FOUND] = MSG_NO_SUCH_FILE,
		[ERR_PIPE] = "Pipe error",
		[ERR_FORK] = "Fork error",
		[ERR_DUP] = "Dup error",
		[ERR_EXEC] = "Execution error"
	};

	return (messages[error]);
}

void	print_error_new(t_error_type error, const char *detail)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (detail)
	{
		ft_putstr_fd(detail, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	ft_putendl_fd((char *)get_error_message(error), STDERR_FILENO);
}

void	handle_error(t_shell *shell, t_error_type error, const char *detail)
{
	int	status;

	status = 1;
	if (error == ERR_CMD_NOT_FOUND_NEW)
		status = ERR_CMD_NOT_FOUND;
	else if (error == ERR_PERMISSION_NEW)
		status = ERR_PERMISSION;
	shell->exit_status = status;
	print_error_new(error, detail);
}

int	set_error_status(t_shell *shell, int status)
{
	shell->exit_status = status;
	return (ERROR);
} 