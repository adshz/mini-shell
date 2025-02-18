/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expand_array_build_utils.c            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 16:06:18 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 23:40:53 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

/*
* Copies the command parts from the split_parts array
* to the expanded_args array  
* 1. Count Phase  
*/
size_t	cmd_count_command_parts(char **split_parts)
{
	size_t	count;

	count = 0;
	while (split_parts[count])
		count++;
	return (count);
}

/*
* Counts the remaining arguments in the args array
* 1. Count Phase
*/
size_t	cmd_count_remaining_args(char **args)
{
	size_t	count;

	count = 0;
	while (args[count + 1])
		count++;
	return (count);
}

/*
* Allocates memory for the expanded_args array
* 2. Allocation Phase
* allocate memory for new array 
*/
char	**cmd_allocate_expanded_args(size_t total_size)
{
	char	**expanded_args;

	expanded_args = malloc(sizeof(char *) * (total_size + 1));
	if (!expanded_args)
		return (NULL);
	expanded_args[total_size] = NULL;
	return (expanded_args);
}

/*
* Copies the command parts from the split_parts array to the expanded_args array
* 2. Copy Phase
*  Copies command parts to new ar
*/
void	cmd_copy_command_parts(char **dest, char **src, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		dest[i] = src[i];
		i++;
	}
}

/*
* Copies the remaining arguments from the src array to the dest array
* 2. Copy Phase
*  Copies remaining arguments
*/
int	cmd_copy_remaining_args(char **dest,
		char **src, size_t dest_start, size_t count)
{
	size_t	i;
	size_t	j;

	i = dest_start;
	j = 1;
	while (j <= count)
	{
		dest[i] = ft_strdup(src[j]);
		if (!dest[i])
			return (1);
		i++;
		j++;
	}
	return (0);
}
