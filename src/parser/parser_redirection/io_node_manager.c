/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_node_manager.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:35:28 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:36:11 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* IO Node Management
 *
 * This file manages IO node creation and manipulation
 *
 */
#include "parser/parser.h"

// Create a new IO node
t_io_node	*ft_create_io_node(t_token_type type, char *value)
{
	t_io_node	*new_io_node;

	new_io_node = (t_node *)ft_calloc(1, sizeof(t_io_node));
	if (!new_io_node)
		return (NULL);
	new_io_node->type = convert_to_io_type(type);
	new_io_node->value = ft_strdup(value);
	if (!new_io_node->value)
		return (free(new_io_node), NULL);
	return (new_io_node);
}

void	append_io_node(t_io_node **lst, t_io_node *new)
{
	t_io_node	*curr_node;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	curr_node = *lst;
	while (curr_node && curr_node->next)
		curr_node = curr_node->next;
	curr_node->next = new;
}

// Clear a list of IO nodes
void	cleanup_io_list(t_io_node	**lst)
{
	t_io_node	*curr_node;
	t_io_node	*next;

	curr_node = *lst;
	if (!curr_node)
		return ;
	while (curr_node)
	{
		free(curr_node->value);
		free_char2(curr_node->expanded_value);
		next = curr_node->next;
		free(curr_node);
		curr_node = next;
	}
	*lst = NULL;
}