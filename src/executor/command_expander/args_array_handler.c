/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_array_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 15:38:47 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 15:51:48 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

static int	convert_exit_status_to_str(t_shell *shell, char **expanded)
{
	char	exit_str[12];

	ft_itoa_buf(shell->exit_status, exit_str);
	*expanded = ft_strdup(exit_str);
	return (*expanded != NULL);
}

static int	process_variable_expansion(t_shell *shell,
		char *dollar_pos, char **expanded)
{
	if (dollar_pos[1] == '?')
	{
		return (convert_exit_status_to_str(shell, expanded));
	}
	*expanded = expand_simple_variable(shell, dollar_pos + 1);	
	return (*expanded != NULL);
}

int	handle_expansion(t_shell *shell, char *dollar_pos,
		char **expanded, bool *was_in_double_quotes)
{
	if (!process_variable_expansion(shell, dollar_pos, expanded))
	{
		shell->in_double_quotes = *was_in_double_quotes;
		return (1);
	}
	return (0);
}

static bool	should_skip_split_for_export(const char *cmd, const char *arg)
{
	return (ft_strcmp(cmd, "export") == 0 && ft_strchr(arg, '='));
}

static int	process_single_argument(t_shell *shell, t_ast_node *node, int *i)
{
	char	*space_pos;

	if (process_single_arg_expansion(shell, &node->args[*i]) != 0)
		return (SHELL_ERROR);
	if (!shell->in_double_quotes && (space_pos = ft_strchr(node->args[*i], ' ')))
	{
		if (should_skip_split_for_export(node->args[0], node->args[*i]))
			return (SHELL_SUCCESS);
		if (*(space_pos + 1) == '-')
			return (handle_arg_split(node, i, space_pos));
	}
	return (SHELL_SUCCESS);
}

int	process_remaining_args(t_shell *shell, t_ast_node *node)
{
	int	i;
	int	ret;

	i = 1;
	while (node->args[i])
	{
		ret = process_single_argument(shell, node, &i);
		if (ret != SHELL_SUCCESS)
			return (ret);
		i++;
	}
	return (SHELL_SUCCESS);
}
