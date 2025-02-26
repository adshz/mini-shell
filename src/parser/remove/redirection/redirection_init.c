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
#include "parser/parser.h"

/**
 * @brief Creates appropriate redirection node based on token type
 * @param type Token type
 * @param file_value File token value
 * @return Redirection node or NULL on failure
 */
static t_ast_node	*init_heredoc_data(t_ast_node *redir_node, \
									const char *file_value)
{
	redir_node->data.content_fd = -1;
	redir_node->data.content_path = NULL;
	redir_node->data.delimiter = ft_strdup(file_value);
	if (!redir_node->data.delimiter)
	{
		free(redir_node);
		return (NULL);
	}
	return (redir_node);
}

static t_ast_node	*init_redirection_by_type(t_token_type type, \
										const char *file_value)
{
	t_ast_node	*redir_node;

	redir_node = NULL;
	if (type == TOKEN_REDIRECT_OUT)
		redir_node = create_ast_node(AST_REDIR_OUT, NULL);
	else if (type == TOKEN_REDIRECT_IN)
		redir_node = create_ast_node(AST_REDIR_IN, NULL);
	else if (type == TOKEN_APPEND)
		redir_node = create_ast_node(AST_REDIR_APPEND, NULL);
	else if (type == TOKEN_HEREDOC)
	{
		redir_node = create_ast_node(AST_HEREDOC, NULL);
		if (redir_node)
			return (init_heredoc_data(redir_node, file_value));
	}
	return (redir_node);
}

t_ast_node	*create_redirection_node(t_token_type type, const char *file_value)
{
	t_ast_node	*redir_node;

	redir_node = init_redirection_by_type(type, file_value);
	if (!redir_node)
		return (NULL);
	return (create_redir_file_node(file_value, redir_node));
}
