/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args_array_split_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:01:52 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 16:05:01 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

static int	count_and_allocate_new_args(char **args, char ***new_args)
{
	int	new_arg_count;

	new_arg_count = 0;
	while (args[new_arg_count])
		new_arg_count++;
	*new_args = malloc(sizeof(char *) * (new_arg_count + 2));
	if (!*new_args)
		return (SHELL_ERROR);
	return (new_arg_count);
}

static void	copy_args_before_split(char **new_args,
		char **old_args, int split_pos)
{
	int	j;

	j = 0;
	while (j < split_pos)
	{
		new_args[j] = old_args[j];
		j++;
	}
}

static void	split_current_arg(char **new_args,
		char *arg, char *space_pos, int pos)
{
	*space_pos = '\0';
	new_args[pos] = ft_strdup(arg);
	new_args[pos + 1] = ft_strdup(space_pos + 1);
}

static void	copy_remaining_args(char **new_args,
		char **old_args, int *i, int start_pos)
{
	int	j;

	j = start_pos;
	while (old_args[*i + 1])
	{
		new_args[j] = old_args[*i + 1];
		(*i)++;
		j++;
	}
	new_args[j] = NULL;
}

int	handle_arg_split(t_ast_node *node, int *i, char *space_pos)
{
	char	**new_args;
	int		new_arg_count;

	new_arg_count = count_and_allocate_new_args(node->args, &new_args);
	if (new_arg_count == SHELL_ERROR)
		return (SHELL_ERROR);
	copy_args_before_split(new_args, node->args, *i);
	split_current_arg(new_args, node->args[*i], space_pos, *i);
	copy_remaining_args(new_args, node->args, i, *i + 2);
	free(node->args[*i]);
	free(node->args);
	node->args = new_args;
	return (SHELL_SUCCESS);
}
