/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander_args_build.c                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:11:17 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 16:13:34 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

/*
* Finalisation Phase
* 1. updates node with new arguments
* 2. free old arguments
* 3. update node value to match the command name
*/
static int	complete_args_update(t_ast_node *node,
		char **expanded_args, char **split_parts)
{
	ft_free_array(node->args);
	node->args = expanded_args;
	free(split_parts);
	free(node->value);
	node->value = ft_strdup(expanded_args[0]);
	ft_putstr_fd("DEBUG: Final expanded arguments:\n", STDERR_FILENO);
	return (0);
}

/*
* Builds a new args array
* 1. Count Phase
* 2. Copy Phase
* 3. Finalisation Phase
*/
int	build_new_args_array(t_ast_node *node, char **split_parts)
{
	char	**expanded_args;
	size_t	split_count;
	size_t	orig_count;
	size_t	total_count;

	split_count = cmd_count_command_parts(split_parts);
	orig_count = cmd_count_remaining_args(node->args);
	total_count = split_count + orig_count;
	expanded_args = cmd_allocate_expanded_args(total_count);
	if (!expanded_args)
	{
		ft_free_array(split_parts);
		return (1);
	}
	cmd_copy_command_parts(expanded_args, split_parts, split_count);
	if (cmd_copy_remaining_args(expanded_args,
			node->args, split_count, orig_count))
	{
		ft_free_array(expanded_args);
		ft_free_array(split_parts);
		return (1);
	}
	expanded_args[total_count] = NULL;
	return (complete_args_update(node, expanded_args, split_parts));
}
