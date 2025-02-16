/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:13:10 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 19:13:47 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "libft.h"
#include "types.h"
#include "./cd.h"

int	handle_cd_error(const char *path, const char *arg)
{
	ft_putstr_fd("cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)arg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	if (path != arg)
		free((void *)path);
	return (1);
}

void	cleanup_path(char *path, const char *arg, t_shell *shell)
{
	if (path != arg && path != hashmap_get(shell->env, "HOME") && \
	path != hashmap_get(shell->env, "OLDPWD"))
		free(path);
}
