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
t_io_node *ft_create_io_node(t_token_type type, char *value);

// Append an IO node to a list
void append_io_node(t_io_node **lst, t_io_node *new);

// Clear a list of IO nodes
void ft_clear_io_list(t_io_node **lst);

