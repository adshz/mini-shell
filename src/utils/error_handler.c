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

static const char	*get_error_message(t_error_type error)
{
	static const char	*messages[] = {
		[ERR_NONE] = "Success",
		[ERR_MALLOC] = "Memory allocation failed",
		[ERR_SYNTAX] = "Syntax error",
		[ERR_CMD_NOT_FOUND] = "Command not found",
		[ERR_PERMISSION] = "Permission denied",
		[ERR_FILE_NOT_FOUND] = "No such file or directory",
		[ERR_PIPE] = "Pipe error",
		[ERR_FORK] = "Fork error",
		[ERR_DUP] = "Dup error",
		[ERR_EXEC] = "Execution error"
	};

	return (messages[error]);
}

void	print_error(t_error_type error, const char *detail)
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
	if (error == ERR_CMD_NOT_FOUND)
		status = 127;
	else if (error == ERR_PERMISSION)
		status = 126;
	shell->exit_status = status;
	print_error(error, detail);
}

int	set_error_status(t_shell *shell, int status)
{
	shell->exit_status = status;
	return (ERROR);
} 