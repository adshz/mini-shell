/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"

t_ast_node	*handle_redirections(t_ast_node *node, t_token **tokens)
{
	t_token	*current;

	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		node = handle_redirection(node, tokens);
		if (!node)
			return (NULL);
		current = *tokens;
	}
	return (node);
}

t_ast_node	*parse_expression(t_token **tokens)
{
	t_ast_node	*node;

	ft_putstr_fd("\nDEBUG: Starting parse_expression\n", STDERR_FILENO);
	if (!tokens || !*tokens)
	{
		ft_putstr_fd("DEBUG: No tokens to parse\n", STDERR_FILENO);
		return NULL;
	}

	node = parse_pipeline(tokens);
	if (!node)
	{
		ft_putstr_fd("DEBUG: Pipeline parsing failed\n", STDERR_FILENO);
		return NULL;
	}

	ft_putstr_fd("DEBUG: Handling redirections for node: ", STDERR_FILENO);
	ft_putstr_fd(node->value ? node->value : "NULL", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	return handle_redirections(node, tokens);
}

t_ast_node	*parse(t_token *tokens)
{
	t_ast_node	*ast;
	t_token		*current;

	if (!tokens)
		return (NULL);

	// Check for pipe at start of input
	if (tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		return (NULL);
	}

	current = tokens;
	ast = parse_expression(&current);
	if (!ast)
	{
		// Don't free tokens here, as they are owned by the shell struct
		return (NULL);
	}
	if (current != NULL)
	{
		free_ast(ast);
		return (NULL);
	}
	return (ast);
}