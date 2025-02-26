/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 command_init_utils.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/19 19:00:54 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/19 19:00:54 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Determines the allocated size for the argument array
 * @param start Starting token
 * @param arg_count Number of arguments to process
 * @return Allocated size
 * 
 * Determines the allocated size for the argument array.
 * If the start token is NULL or the value is NULL, 
 * the allocated size is the argument count.
 * If the start token is "exit" and the argument count is less than 3, 
 * the allocated size is 3.
 * Otherwise, the allocated size is the argument count.
 *
 * @note as for 3, exit, exit code, null.
 */
static int	determine_allocated_size(t_token *start, int arg_count)
{
	if (!start || !start->value)
		return (arg_count);
	if (ft_strcmp(start->value, "exit") == 0)
	{
		if (arg_count < 3)
			return (3);
	}
	return (arg_count);
}

static int	copy_token_values(char **args, t_token *current, int count)
{
	int	i;

	i = 0;
	while (i < count && current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
			return (0);
		current = current->next;
		i++;
	}
	return (i);
}

static void	fill_remaining_args(char **args, int start_index, int total_size)
{
	while (start_index < total_size)
	{
		args[start_index] = NULL;
		start_index++;
	}
	args[total_size] = NULL;
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
int	fill_args(char **args, t_token *start, int arg_count)
{
	int	allocated_size;
	int	copied_count;

	allocated_size = determine_allocated_size(start, arg_count);
	copied_count = copy_token_values(args, start, arg_count);
	if (!copied_count && arg_count > 0)
		return (0);
	fill_remaining_args(args, copied_count, allocated_size);
	return (1);
}
