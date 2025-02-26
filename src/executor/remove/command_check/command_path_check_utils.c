/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_check_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:59:01 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 15:00:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static int	get_access_error_message(const char *cmd)
{
	if (access(cmd, F_OK) != 0)
		return (print_error((char *)cmd, ERR_MSG_NO_SUCH_FILE, ERR_NOT_FOUND));
	if (access(cmd, X_OK) != 0)
		return (print_error((char *)cmd, ERR_MSG_NOT_EXECUTABLE, \
					ERR_NOT_EXECUTABLE));
	return (0);
}

static int	is_directory(const char *path)
{
	int	fd;

	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}

int	check_command_path(const char *cmd)
{
	if (!cmd)
		return (print_error(NULL, ERR_MSG_INVALID_CMD, ERR_INVALID_ARG));
	if (access(cmd, F_OK) == 0)
	{
		if (is_directory(cmd))
			return (print_error((char *)cmd, ERR_MSG_IS_DIR, ERR_IS_DIR));
		if (access(cmd, X_OK) != 0)
			return (print_error((char *)cmd, ERR_MSG_NOT_EXECUTABLE, \
					ERR_NOT_EXECUTABLE));
		return (0);
	}
	return (get_access_error_message(cmd));
}
