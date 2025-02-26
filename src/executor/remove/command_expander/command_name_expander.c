/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:12:58 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 16:15:25 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static int	cmd_handle_suffix_combination(char **expanded,
		char *var_end)
{
	char	*suffix;
	char	*temp;

	suffix = ft_strdup(var_end);
	if (!suffix)
		return (1);
	if (*suffix)
	{
		temp = *expanded;
		*expanded = ft_strjoin(*expanded, suffix);
		free(temp);
		if (!*expanded)
		{
			free(suffix);
			return (1);
		}
	}
	free(suffix);
	return (0);
}

/* expand command name with variable expand_command_name_with_var
* Phase 4: Command Processing
* cmd_process_expanded_value
* - Input: expanded value, prefix, AST node
* - Responsibility: Split and rebuild command array
* - Output: updated command array
*/
static int	cmd_process_expanded_value(char *expanded,
		char *prefix, t_ast_node *node)
{
	char	**split_parts;
	char	*final_command;

	split_parts = ft_split(expanded, ' ');
	free(expanded);
	if (!split_parts)
	{
		free(prefix);
		return (1);
	}
	final_command = NULL;
	if (prefix)
	{
		final_command = ft_strjoin(prefix, split_parts[0]);
		free(prefix);
		free(split_parts[0]);
		if (!final_command)
		{
			ft_free_array(split_parts);
			return (1);
		}
		split_parts[0] = final_command;
	}
	return (build_new_args_array(node, split_parts));
}

/*
* expand command name with variable expand_command_name_with_var
* there are four phases in this function
* Phase 1: prefix handling (cmd_handle_prefix_extraction)
* Phase 2: variable expansion (cmd_handle_variable_expansion)
* Phase 3: suffix combination (cmd_handle_suffix_combination)
* Phase 4: process expanded value (cmd_process_expanded_value)
*/
static int	expand_command_variable_part(t_shell *shell,
		t_ast_node *node, char *dollar_pos, char *prefix)
{
	char	*expanded;
	char	*var_end;
	int		ret;

	expanded = cmd_handle_variable_expansion(shell, dollar_pos, &var_end);
	if (!expanded)
	{
		free(prefix);
		return (1);
	}
	ret = cmd_handle_suffix_combination(&expanded, var_end);
	if (ret)
	{
		free(prefix);
		free(expanded);
		return (1);
	}
	ret = cmd_process_expanded_value(expanded, prefix, node);
	return (ret);
}

int	expand_command_name_with_var(t_shell *shell,
		t_ast_node *node, char *dollar_pos)
{
	char	*prefix;
	int		ret;

	if (!shell || !node || !dollar_pos)
		return (1);
	prefix = NULL;
	ret = cmd_handle_prefix_extraction(node->args[0], dollar_pos, &prefix);
	if (ret)
		return (ret);
	return (expand_command_variable_part(shell, node, dollar_pos, prefix));
}
