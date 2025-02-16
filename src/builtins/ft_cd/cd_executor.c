/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_executor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 18:41:35 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 19:11:25 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cd.h"

static int	handle_cd_with_path_arg(t_shell *shell, t_ast_node *node)
{
	char	*expanded_path;
	int		ret;

	expanded_path = NULL;
	ret = 0;
	if (node->args[1][0] == '~')
		expanded_path = expand_tilde(shell, node->args[1]);
	else
		expanded_path = handle_path(shell, node->args[1]);
	if (!expanded_path)
		return (1);
	ret = handle_cd_path(shell, expanded_path);
	free(expanded_path);
	return (ret);
}

int	builtin_cd(t_shell *shell, t_ast_node *node)
{
	char	*path;

	path = NULL;
	if (!node->args[1])
		path = get_home_path(shell);
	else if (ft_strcmp(node->args[1], "-") == 0)
		path = get_oldpwd_path(shell);
	else
		return (handle_cd_with_path_arg(shell, node));
	if (!path)
		return (1);
	return (handle_cd_path(shell, path));
}

int	handle_cd_path(t_shell *shell, const char *path)
{
	char	*old_pwd;

	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
	{
		ft_putendl_fd(ERR_GETCWD ERR_NO_FILE, STDERR_FILENO);
		return (1);
	}
	if (chdir(path) == -1)
	{
		free(old_pwd);
		return (handle_cd_error(path, path));
	}
	update_pwd_vars(shell, old_pwd);
	free(old_pwd);
	cleanup_path((char *)path, path, shell);
	return (0);
}
