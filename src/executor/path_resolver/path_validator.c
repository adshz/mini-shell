/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 17:49:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 17:49:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

t_err	check_executable(char *file, bool is_cmd)
{
	if (!*file)
		return ((t_err){ERRNO_GENERAL, MSGERR_NO_SUCH_FILE, file});
	if (access(file, F_OK) == 0)
	{
		if (access(file, X_OK) == -1)
			return ((t_err){ERRNO_CANT_EXEC, MSGERR_PERM_DENIED, file});
		return ((t_err){ERRNO_NONE, 42, NULL});
	}
	if (is_cmd)
		return ((t_err){ERRNO_NOT_FOUND, MSGERR_CMD_NOT_FOUND, file});
	return ((t_err){ERRNO_NOT_FOUND, MSGERR_NO_SUCH_FILE, file});
}
