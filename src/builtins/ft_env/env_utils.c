/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:54:17 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:54:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "env.h"

void	env_print_env_var(const char *key, const char *value)
{
	char	*key_copy;
	char	*value_copy;

	key_copy = ft_strdup(key);
	value_copy = ft_strdup(value);
	ft_putstr_fd(key_copy, STDOUT_FILENO);
	ft_putstr_fd("=", STDOUT_FILENO);
	ft_putendl_fd(value_copy, STDOUT_FILENO);
	free(key_copy);
	free(value_copy);
}

int	handle_env_parent_process(pid_t pid, char *cmd_path)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	free(cmd_path);
	return ((status >> 8) & 0xff);
}

int	handle_env_child_process(char *cmd_path, \
							char **args, char **env_array)
{
	execve(cmd_path, args, env_array);
	perror("minishell: execve");
	free(cmd_path);
	exit(127);
}

int	handle_env_command_not_found(const char *cmd)
{
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putendl_fd(": No such file or directory", STDERR_FILENO);
	return (127);
}
