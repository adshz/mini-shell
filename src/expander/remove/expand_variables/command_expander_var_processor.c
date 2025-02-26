/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expander_var_processor.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:36:21 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 15:36:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

static int	cmd_expand_extract_prefix(t_cmd_expand *exp, const char *cmd)
{
	if (cmd[0] == '$')
		return (1);
	exp->dollar_pos = ft_strchr(cmd, '$');
	if (!exp->dollar_pos)
		return (1);
	exp->prefix_len = exp->dollar_pos - cmd;
	exp->prefix = ft_substr(cmd, 0, exp->prefix_len);
	if (!exp->prefix)
		return (0);
	return (1);
}

static int	cmd_expand_variable_part(t_cmd_expand *exp, t_shell *shell, \
								const char *cmd)
{
	size_t	offset;

	offset = 0;
	if (exp->prefix)
		offset = ft_strlen(exp->prefix);
	exp->expanded_value = expand_simple_variable(shell, cmd + offset + 1);
	if (!exp->expanded_value)
	{
		free(exp->prefix);
		return (0);
	}
	return (1);
}

static int	cmd_expand_combine_parts(t_cmd_expand *exp)
{
	if (!exp->prefix)
		return (1);
	exp->full_cmd = ft_strjoin(exp->prefix, exp->expanded_value);
	free(exp->prefix);
	free(exp->expanded_value);
	if (!exp->full_cmd)
		return (0);
	exp->expanded_value = exp->full_cmd;
	return (1);
}

static char	**cmd_expand_create_command_args(t_cmd_expand *exp)
{
	exp->cmd_args = ft_split(exp->expanded_value, ' ');
	free(exp->expanded_value);
	if (!exp->cmd_args || !exp->cmd_args[0])
	{
		if (exp->cmd_args)
			ft_free_array(exp->cmd_args);
		return (NULL);
	}
	return (exp->cmd_args);
}

char	**handle_variable_command(t_shell *shell, const char *cmd)
{
	t_cmd_expand	exp;

	init_cmd_expand(&exp);
	if (!cmd_expand_extract_prefix(&exp, cmd))
		return (NULL);
	if (!cmd_expand_variable_part(&exp, shell, cmd))
		return (NULL);
	if (!cmd_expand_combine_parts(&exp))
		return (NULL);
	return (cmd_expand_create_command_args(&exp));
}
