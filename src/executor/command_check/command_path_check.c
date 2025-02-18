/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_check.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static int get_access_error_message(const char *cmd)
{
	if (access(cmd, F_OK) != 0)
		return (print_error((char *)cmd, ERR_MSG_NO_SUCH_FILE, ERR_NOT_FOUND));
	if (access(cmd, X_OK) != 0)
		return (print_error((char *)cmd, ERR_MSG_NOT_EXECUTABLE, ERR_NOT_EXECUTABLE));
	return (0);
}

static int is_directory(const char *path)
{
	int fd;
	
	fd = open(path, O_DIRECTORY);
	if (fd >= 0)
	{
		close(fd);
		return (1);
	}
	return (0);
}

int check_command_path(const char *cmd)
{
	if (!cmd)
		return (print_error(NULL, ERR_MSG_INVALID_CMD, ERR_INVALID_ARG));

	if (access(cmd, F_OK) == 0)
	{
		if (is_directory(cmd))
			return (print_error((char *)cmd, ERR_MSG_IS_DIR, ERR_IS_DIR));
		if (access(cmd, X_OK) != 0)
			return (print_error((char *)cmd, ERR_MSG_NOT_EXECUTABLE, ERR_NOT_EXECUTABLE));
		return (0);
	}
	return (get_access_error_message(cmd));
}

int	is_direct_path(const char *cmd)
{
	if (!cmd)
		return (0);
	return (cmd[0] == '/' || cmd[0] == '.');
}

char	*get_path_env(t_shell *shell)
{
	char	*path;

	path = hashmap_get(shell->env, "PATH");
	if (!path)
	{
		handle_error(shell, ERROR_FILE_NOT_FOUND, "PATH");
		return (NULL);
	}
	return (path);
}

int	is_special_path(const char *cmd)
{
	if (!cmd || !*cmd)
		return (0);
	if (ft_strchr(cmd, '/'))
		return (1);
	if (ft_strcmp(cmd, "..") == 0 || ft_strcmp(cmd, ".") == 0)
		return (1);
	return (0);
}

int	handle_direct_path(t_shell *shell, const char *cmd)
{
	if (access(cmd, F_OK) == 0)
	{
		if (access(cmd, X_OK) != 0)
			return (print_error((char *)cmd, ERR_MSG_NOT_EXECUTABLE, ERR_NOT_EXECUTABLE));
		return (0);
	}
	shell->exit_status = ERR_NOT_FOUND;
	return (print_error((char *)cmd, ERR_MSG_CMD_NOT_FOUND, ERR_NOT_FOUND));
}
