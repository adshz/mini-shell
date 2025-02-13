/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_path_resolver.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:56:36 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 09:07:25 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

char	*find_command(t_shell *shell, const char *cmd)
{
	char	*path;
	char	**paths;

	if (!cmd || !shell)
		return (NULL);
	if (is_direct_path(cmd))
		return (ft_strdup(cmd));
	path = get_path_env(shell);
	if (!path)
		return (NULL);
	paths = split_path_env(shell, path);
	if (!paths)
		return (NULL);
	return (search_in_paths(shell, paths, cmd));
}

char	**split_path_env(t_shell *shell, const char *path)
{
	char	**paths;

	paths = ft_split(path, ':');
	if (!paths)
	{
		handle_error(shell, ERR_MALLOC, NULL);
		return (NULL);
	}
	return (paths);
}

int	handle_path_resolution(t_shell *shell, t_ast_node *node, char **cmd_path)
{
	int	ret;

	*cmd_path = get_command_path(shell, node->args[0], shell->env);
	if (!*cmd_path && ft_strchr(node->args[0], '/'))
	{
		ret = handle_direct_path(shell, node->args[0]);
		if (ret == 0)
			*cmd_path = ft_strdup(node->args[0]);
		else
			return (ret);
	}
	else if (!*cmd_path)
		return (handle_command_not_found(shell, node->args[0]));
	return (0);
}
