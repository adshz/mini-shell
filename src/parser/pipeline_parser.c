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

t_ast_node	*parse_pipeline(t_token **tokens)
{
	t_ast_node	*left;
	t_ast_node	*right;
	t_token		*current;

	if (!tokens || !*tokens)
		return (NULL);

	// Parse left side (can be a command or redirection)
	left = parse_command(tokens);
	if (!left)
		return (NULL);

	// Handle any redirections on the left side
	left = handle_redirections(left, tokens);
	if (!left)
		return (NULL);

	current = *tokens;
	if (!current || current->type != TOKEN_PIPE)
		return (left);

	// Move past pipe token
	*tokens = current->next;
	if (!*tokens)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		free_ast(left);
		return (NULL);
	}

	// Parse right side recursively
	right = parse_pipeline(tokens);
	if (!right)
	{
		free_ast(left);
		return (NULL);
	}

	// Create pipe node
	t_ast_node *pipe_node = create_ast_node(AST_PIPE, NULL);
	if (!pipe_node)
	{
		free_ast(left);
		free_ast(right);
		return (NULL);
	}

	pipe_node->left = left;
	pipe_node->right = right;
	return pipe_node;
} 