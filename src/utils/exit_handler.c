/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:02:55 by szhong            #+#    #+#             */
/*   Updated: 2025/02/22 11:44:26 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "errors.h"

void	exit_handler(t_shell *shell, char *param, int err, int is_exit)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (err == INV_ARGS)
		ft_putstr_fd("No arguments needed", STDERR_FILENO);
	else if (err == HASH_FULL)
		ft_putstr_fd("Insert error: Hash Table is full", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
		error_print(param, err);
}
