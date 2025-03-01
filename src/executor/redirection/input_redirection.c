/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_redirection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:54:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:55:52 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_err	ft_validate_read_perm(char *file)
{
	if (!*file)
		return ((t_err){ERRNO_GENERAL, MSGERR_PERM_DENIED, file});
	if (access(file, F_OK) == 0)
	{
		if (access(file, R_OK) == -1)
			return ((t_err){ERRNO_GENERAL, MSGERR_PERM_DENIED, file});
		return ((t_err){ERRNO_NONE, 42, NULL});
	}
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, file});
}

int	ft_handle_input_redirection(t_io_node *io_lst, int *status)
{
	int	fd;

	if (!io_lst->expaned_value || io_lst->expaned_value[1])
	{
		*status = exec_print_err((t_err){ERRNO_GENERAL, MSGERR_AMBIGUOUS, 
			\io_lst->value});
		return (*status);
	}
	fd = open(io_lst->expanded_value[0], O_RDONLY);
	if (fd == -1)
	{
		*status = exec_print_err(\
						ft_validate_read_perm(io_lst->expanded_value[0]));
		return (*status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}
