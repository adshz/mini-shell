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

static void	free_args(char **args)
{
	int	i;

	if (!args)
		return ;
	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

void	free_ast(t_ast_node *node)
{
	if (!node || node == (void *)-1)
		return ;
    ft_printf("Freeing AST node: %p\n", (void*)node);  // Debug print
	if (node->left)
		free_ast(node->left);
	if (node->right)
		free_ast(node->right);
	if (node->value)
	{
        ft_printf("Freeing value: %s\n", node->value);  // Debug print
        free(node->value);
	}
	if (node->args)
    {
        ft_printf("Freeing args\n");  // Debug print
		free_args(node->args);
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


