/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expander.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 17:22:27 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 17:46:47 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

void	init_cmd_expand(t_cmd_expand *exp)
{
	exp->expanded_value = NULL;
	exp->cmd_args = NULL;
	exp->prefix = NULL;
	exp->dollar_pos = NULL;
	exp->full_cmd = NULL;
	exp->prefix_len = 0;
}

static char	**handle_regular_command(const char *cmd)
{
	char	**cmd_args;

	cmd_args = malloc(2 * sizeof(char *));
	if (!cmd_args)
		return (NULL);
	cmd_args[0] = ft_strdup(cmd);
	cmd_args[1] = NULL;
	return (cmd_args);
}

char	**expand_command(t_shell *shell, const char *cmd)
{
	if (cmd[0] == '$')
		return (handle_variable_command(shell, cmd));
	return (handle_regular_command(cmd));
}
