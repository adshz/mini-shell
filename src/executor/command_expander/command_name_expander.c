/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:12:58 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 11:29:31 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"
#include <stdatomic.h>

// Handles the core command name expansion logic
// entry point for command name expansion is expand_command_args
// it will call handle_variable_expansion where the command name is expanded
// handle_variable_expansion will call expand_command_name_with_var
// or process_remaining_args where the rest of the arguments are expanded
//
// expand_command_name_with_var 
//  - create_command_prefix to extract the prefix of the command name
//  - process_variable_expansion to process the expanded command name
//  - process_expanded_value will call build_new_args_array to update the command name
//      - build_new_args_array will call complete_args_update to update the command name
//          - complete_args_update will call replace_argument to update the command name
//
// （argument expansion flow:
//  - process_single_arg_expansion will call expand_command_name_with_var to expand the command name
//  - expand_command_name_with_var will call process_expanded_value to process the expanded command name
//  - process_expanded_value will call build_new_args_array to update the command name
//      - build_new_args_array will call complete_args_update to update the command name
//          - complete_args_update will call replace_argument to update the command name
//
// and then call process_remaining_args to process the rest of the arguments
//// process_remaining_args will call process_single_arg_expansion to process the rest 
/*
* expand_command_name_with_var
* ├── cmd_handle_prefix_extraction
* │   └── create_command_prefix
* ├── cmd_handle_variable_expansion
* │   ├── handle_exit_status
* │   │   └── ft_itoa_buf
* │   └── extract_variable_name
* ├── cmd_handle_suffix_combination
* └── cmd_process_expanded_value
*     ├── ft_split
*     ├── ft_strjoin (for prefix)
*     └── build_new_args_array
*        ├── cmd_count_command_parts
*        ├── cmd_count_remaining_args
*        ├── cmd_allocate_expanded_args
*        ├── cmd_copy_command_parts
*        ├── cmd_copy_remaining_args
*        └── complete_args_update
*/

/* expand command name with variable expand_command_name_with_var
* Phase 3: Suffix combination
* cmd_handle_suffix_combination
* - Input: expanded value, variable end position
* - Responsibility: Combine expanded value with suffix
* - Output: final expanded string
*/
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
	// Split expanded value into parts
	split_parts = ft_split(expanded, ' ');
	free(expanded);
	if (!split_parts)
	{
		free(prefix);
		return (1);
	}
	// Combine prefix with first part if needed
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
	ft_putstr_fd("DEBUG: Command name contains variable\n",
		STDERR_FILENO);
	ret = cmd_handle_prefix_extraction(node->args[0],
			dollar_pos, &prefix);
	if (ret)
		return (ret);
	expanded = cmd_handle_variable_expansion(shell,
			dollar_pos, &var_end);
	if (!expanded)
	{
		free(prefix);
		return (1);
	}
	if (cmd_handle_suffix_combination(&expanded, var_end))
	{
		free(prefix);
		free(expanded);
		return (1);
	}
	return (cmd_process_expanded_value(expanded,
			prefix, node));
}
