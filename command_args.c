/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_args.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:06:31 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 19:08:51 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * @brief Allocates memory for argument array
 * @param arg_count Number of arguments to allocate
 * @return Allocated array or NULL on failure
 * 
 * Allocates memory for arg_count + 1 pointers, with the last
 * pointer set to NULL as a terminator.
 */
char	**allocate_args(int arg_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	return (args);
}

/**
 * @brief Fills argument array with duplicated token values
 * @param args Array to fill with arguments
 * @param start Starting token
 * @param arg_count Number of arguments to process
 * @return 1 on success, 0 on failure
 * 
 * Duplicates token values into the args array. Handles both
 * regular tokens and variable tokens. Ensures proper memory
 * allocation for each argument.
 */
static int	get_allocated_size(t_token *start, int arg_count)
{
	if (start && start->value && ft_strcmp(start->value, "exit") == 0)
	{
		if (arg_count < 3)
			return (3);
	}
	return (arg_count);
}

static void	fill_null_args(char **args, int start_idx, int end_idx)
{
	while (start_idx < end_idx)
	{
		args[start_idx] = NULL;
		start_idx++;
	}
}

int	fill_args(char **args, t_token *start, int arg_count)
{
	t_token	*current;
	int		i;
	int		allocated_size;

	current = start;
	i = 0;
	allocated_size = get_allocated_size(start, arg_count);
	while (i < arg_count && current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
			return (0);
		current = current->next;
		i++;
	}
	fill_null_args(args, i, allocated_size);
	args[allocated_size] = NULL;
	return (1);
}
