/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_parser.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"

static t_ast_node	*create_pipe_node(t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*pipe_node;

	pipe_node = create_ast_node(AST_PIPE, NULL);
	if (!pipe_node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}
	pipe_node->left = left;
	pipe_node->right = right;
	return (pipe_node);
}

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);

	// Check for pipe at start of input
	if ((*tokens)->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (NULL);
	}

	left = parse_command(tokens);
	if (!left)
		return (NULL);

	// Check if exit is the first command and there's a pipe
	if (left->value && ft_strcmp(left->value, "exit") == 0 && *tokens && (*tokens)->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		free_ast(left);
		return (NULL);
	}

	current = *tokens;
	if (!current || current->type != TOKEN_PIPE)
		return (left);

	*tokens = current->next;
	right = parse_command(tokens);
	if (!right)
	{
		free_ast(left);
		return (NULL);
	}

	current = *tokens;
	if (current && current->type == TOKEN_PIPE)
	{
		t_ast_node *rest_of_pipeline;
		*tokens = current->next;
		rest_of_pipeline = parse_pipeline(tokens);
		if (!rest_of_pipeline)
		{
			free_ast(left);
			free_ast(right);
			return (NULL);
		}
		right = create_pipe_node(right, rest_of_pipeline);
		if (!right)
		{
			free_ast(left);
			return (NULL);
		}
	}

	return (create_pipe_node(left, right));
} 