/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_init.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:39:44 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 13:39:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"

/**
 * @brief Creates appropriate redirection node based on token type
 * @param type Token type
 * @param file_value File token value
 * @return Redirection node or NULL on failure
 */
t_ast_node	*create_redirection_node(t_token_type type, \
										const char *file_value)
{
	t_ast_node	*redir_node;
	t_ast_node	*file_node;

	if (type == TOKEN_REDIRECT_OUT)
		redir_node = create_ast_node(AST_REDIR_OUT, NULL);
	else if (type == TOKEN_REDIRECT_IN)
		redir_node = create_ast_node(AST_REDIR_IN, NULL);
	else if (type == TOKEN_APPEND)
		redir_node = create_ast_node(AST_REDIR_APPEND, NULL);
	else if (type == TOKEN_HEREDOC)
		redir_node = create_ast_node(AST_HEREDOC, NULL);
	else
		redir_node = create_ast_node(AST_HEREDOC, NULL);
	if (!redir_node)
		return (NULL);
	file_node = create_ast_node(AST_COMMAND, (char *)file_value);
	if (!file_node)
	{
		free_ast(redir_node);
		return (NULL);
	}
	redir_node->right = file_node;
	return (redir_node);
}

/**
 * @brief Creates a command node from a token
 * @param token Token containing command value
 * @return Command node or NULL on failure
 */
t_ast_node	*create_command_from_token(t_token *token)
{
	t_ast_node	*cmd_node;

	cmd_node = create_ast_node(AST_COMMAND, token->value);
	if (!cmd_node)
		return (NULL);
	cmd_node->args = malloc(sizeof(char *) * 2);
	if (!cmd_node->args)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	cmd_node->args[0] = ft_strdup(token->value);
	cmd_node->args[1] = NULL;
	return (cmd_node);
}

/**
 * @brief Attaches a command node to the result tree
 * @param result Current result tree
 * @param cmd_node Command node to attach
 * @return Updated result tree
 */
t_ast_node	*attach_command_node(t_ast_node *result, \
									t_ast_node *cmd_node)
{
	t_ast_node	*current_node;

	if (!result)
		return (cmd_node);
	current_node = result;
	while (current_node->left)
		current_node = current_node->left;
	current_node->left = cmd_node;
	return (result);
}
