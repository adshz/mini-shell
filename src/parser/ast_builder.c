/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_builder.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "parser.h"
#include "libft.h"
#include <stdio.h>

t_ast_node	*create_ast_node(t_ast_type type, char *value)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	node->value = NULL;
	if (value)
	{
		node->value = ft_strdup(value);
		if (!node->value)
		{
			free(node);
			return (NULL);
		}
	}
	return (node);
}

void	free_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == AST_PIPE)
	{
		free_ast(node->left);
		free_ast(node->right);
	}
	else if (node->type == AST_REDIR_IN || node->type == AST_REDIR_OUT ||
			node->type == AST_REDIR_APPEND || node->type == AST_HEREDOC)
	{
		free_ast(node->left);
		if (node->value)
			free(node->value);
	}
	else if (node->type == AST_COMMAND)
	{
		if (node->value)
			free(node->value);
		if (node->args)
		{
			char **args = node->args;
			while (*args)
				free(*args++);
			free(node->args);
		}
	}
	free(node);
}

t_ast_type	get_ast_type(t_token_type type)
{
	if (type == TOKEN_PIPE)
		return (AST_PIPE);
	if (type == TOKEN_REDIRECT_IN)
		return (AST_REDIR_IN);
	if (type == TOKEN_REDIRECT_OUT)
		return (AST_REDIR_OUT);
	if (type == TOKEN_APPEND)
		return (AST_REDIR_APPEND);
	if (type == TOKEN_HEREDOC)
		return (AST_HEREDOC);
	if (type == TOKEN_AND)
		return (AST_AND);
	if (type == TOKEN_OR)
		return (AST_OR);
	return (AST_COMMAND);
}