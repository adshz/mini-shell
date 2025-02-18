/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:08:17 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:09:45 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

int	handle_tilde_directory(const char *expanded)
{
	if (access(expanded, F_OK) == 0)
	{
		ft_putstr_fd((char *)expanded, STDERR_FILENO);
		ft_putendl_fd(": is a directory", STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	handle_tilde_path(t_shell *shell, t_ast_node *node)
{
	char	*expanded;
	int		ret;

	expanded = expand_tilde(shell, node->args[0]);
	if (!expanded)
	{
		shell->exit_status = ERR_NOT_FOUND;
		return (print_error(node->args[0], MSG_CMD_NOT_FOUND, ERR_NOT_FOUND));
	}
	ret = handle_tilde_directory(expanded);
	free(expanded);
	return (ret);
}

int	handle_dot_paths(t_shell *shell, const char *arg)
{
	if (ft_strcmp(arg, "..") == 0 || ft_strcmp(arg, ".") == 0)
	{
		shell->exit_status = ERR_NOT_FOUND;
		return (print_error((char *)arg, MSG_CMD_NOT_FOUND, ERR_NOT_FOUND));
	}
	return (0);
}

int	handle_special_paths(t_shell *shell, t_ast_node *node)
{
	int	ret;

	if (node->args[0][0] == '~')
	{
		ret = handle_tilde_path(shell, node);
		if (ret != 0)
			return (ret);
	}
	return (handle_dot_paths(shell, node->args[0]));
}
