/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   astnode_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/21 09:51:13 by szhong            #+#    #+#             */
/*   Updated: 2025/02/21 09:51:17 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils.h"

static void	cleanup_ast_node_strings(t_ast_node *node)
{
	if (node->args)
	{
		ft_free_array(node->args);
		node->args = NULL;
	}
	if (node->value)
	{
		free(node->value);
		node->value = NULL;
	}
	if (node->original)
	{
		free(node->original);
		node->original = NULL;
	}
}

static void	cleanup_ast_node_content(t_ast_node *node)
{
	if (node->data.content_path)
	{
		free(node->data.content_path);
		node->data.content_path = NULL;
	}
	if (node->type == AST_HEREDOC && node->data.delimiter)
	{
		free((char *)node->data.delimiter);
		node->data.delimiter = NULL;
	}
}

void	cleanup_ast_node_data(t_ast_node *node)
{
	cleanup_ast_node_strings(node);
	cleanup_ast_node_content(node);
	free(node);
}
