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
#include <stdatomic.h>

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
	if (prefix)
	{
		final_command = ft_strjoin(prefix, split_parts[0]);
		free(prefix);
		free(split_parts[0]);
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
int	expand_command_name_with_var(t_shell *shell,
		t_ast_node *node, char *dollar_pos)
{
	char	*prefix;
	char	*expanded;
	char	*var_end;
	int		ret;

	prefix = NULL;
	expanded = NULL;
	var_end = NULL;
	ft_printf("\nDEBUG [cmd_expand]: Starting command name expansion\n");
	ft_printf("DEBUG [cmd_expand]: Original command: [%s]\n", node->args[0]);
	
	ret = cmd_handle_prefix_extraction(node->args[0],
			dollar_pos, &prefix);
	if (ret)
	{
		ft_printf("DEBUG [cmd_expand]: Prefix extraction failed\n"); // debug deleted
		return (ret);
	}
	ft_printf("DEBUG [cmd_expand]: Extracted prefix: [%s]\n", prefix ? prefix : "NULL");

	expanded = cmd_handle_variable_expansion(shell,
			dollar_pos, &var_end);
	if (!expanded)
	{
		ft_printf("DEBUG [cmd_expand]: Variable expansion failed\n");
		free(prefix);
		return (1);
	}
	ft_printf("DEBUG [cmd_expand]: Expanded value: [%s]\n", expanded);
	ft_printf("DEBUG [cmd_expand]: Var end position: [%s]\n", var_end ? var_end : "NULL");

	if (cmd_handle_suffix_combination(&expanded, var_end))
	{
		ft_printf("DEBUG [cmd_expand]: Suffix combination failed\n");
		free(prefix);
		free(expanded);
		return (1);
	}
	ft_printf("DEBUG [cmd_expand]: After suffix combination: [%s]\n", expanded);

	ret = cmd_process_expanded_value(expanded, prefix, node);
	ft_printf("DEBUG [cmd_expand]: Final command name: [%s]\n", node->args[0]);
	return (ret);
	// return (cmd_process_expanded_value(expanded,
	// 		prefix, node));
}
