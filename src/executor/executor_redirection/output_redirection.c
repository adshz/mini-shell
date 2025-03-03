/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output_redirection.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:57:11 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:58:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_err	ft_validate_write_perm(char *file)
{
	if (!*file)
		return ((t_err){ERRNO_GENERAL, MSGERR_NO_SUCH_FILE, file});
	if (access(file, F_OK) == 0)
	{
		if (access(file, W_OK) == -1)
			return ((t_err){ERRNO_GENERAL, MSGERR_PERM_DENIED, file});
		return ((t_err){ERRNO_NONE, 42, NULL});
	}
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, file});
}

int	ft_handle_output_redirection(t_io_node *io_lst, int *status)
{
	int	fd;

	if (!io_lst->expanded_value || io_lst->expanded_value[1])
	{
		*status = exec_print_err(\
						(t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS, \
						io_lst->value});
		return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		*status = exec_print_err(\
						ft_validate_write_perm(io_lst->expanded_value[0]));
		return (*status);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}

int	ft_handle_append_redirection(t_io_node *io_lst, int *status)
{
	int	fd;

	if (!io_lst->expanded_value || io_lst->expanded_value[1])
	{
		*status = exec_print_err(\
						(t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS, \
						io_lst->value});
		return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		*status = exec_print_err(\
						   ft_validate_write_perm(io_lst->expanded_value[0]));
		return (*status);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}
