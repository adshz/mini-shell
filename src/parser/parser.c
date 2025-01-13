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

t_ast_node	*handle_redirections(t_ast_node *node, t_token **tokens, t_shell *shell)
{
	t_token	*current;

	current = *tokens;
	while (current && is_redirection_token(current->type))
	{
		// Check for consecutive redirection operators
		if (current->next && is_redirection_token(current->next->type))
		{
			ft_putstr_fd("minishell: syntax error near unexpected token `", STDERR_FILENO);
			if (current->next->type == TOKEN_REDIRECT_OUT)
				ft_putstr_fd(">", STDERR_FILENO);
			else if (current->next->type == TOKEN_REDIRECT_IN)
				ft_putstr_fd("<", STDERR_FILENO);
			else if (current->next->type == TOKEN_APPEND)
				ft_putstr_fd(">>", STDERR_FILENO);
			else if (current->next->type == TOKEN_HEREDOC)
				ft_putstr_fd("<<", STDERR_FILENO);
			ft_putendl_fd("'", STDERR_FILENO);
			shell->exit_status = 258;  // Set exit status for syntax error
			return (NULL);
		}

		// Check for missing filename after redirection
		if (!current->next)
		{
			ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
			shell->exit_status = 258;  // Set exit status for syntax error
			return (NULL);
		}

		node = handle_redirection(node, tokens);
		if (!node)
			return (NULL);
		current = *tokens;
	}
	return (node);
}

t_ast_node	*parse_expression(t_token **tokens, t_shell *shell)
{
	t_ast_node	*node;

	if (!tokens || !*tokens)
		return (NULL);

	node = parse_pipeline(tokens, shell);
	if (!node)
		return (NULL);

	if (*tokens && (*tokens)->type == TOKEN_REDIRECT_IN)
		node = handle_redirections(node, tokens, shell);

	return (node);
}

t_ast_node	*parse(t_token *tokens, t_shell *shell)
{
	t_ast_node	*ast;
	t_token		*current;

	if (!tokens)
		return (NULL);

	// Check for pipe at start of input
	if (tokens->type == TOKEN_PIPE)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n", STDERR_FILENO);
		shell->exit_status = 258;  // Set exit status for syntax error
		return (NULL);
	}

	current = tokens;
	ast = parse_expression(&current, shell);
	if (!ast)
	{
		// Set exit status for syntax errors
		if (current && is_redirection_token(current->type))
		{
			shell->exit_status = 258;
		}
		return (NULL);
	}
	if (current != NULL)
	{
		free_ast(ast);
		shell->exit_status = 258;  // Set exit status for any trailing tokens
		return (NULL);
	}
	return (ast);
}