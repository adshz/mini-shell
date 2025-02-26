/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 13:40:07 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 13:40:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

int	check_input_file(const char *filename)
{
	if (access(filename, F_OK) == -1)
		return (0);
	if (access(filename, R_OK) == -1)
		return (0);
	return (1);
}

static bool	validate_file_token(t_token *file_token)
{
	if (!file_token || file_token->type != TOKEN_WORD)
	{
		ft_putendl_fd(\
				"minishell: syntax error near unexpected token `newline'", \
				STDERR_FILENO);
		return (false);
	}
	return (true);
}

static t_ast_node	*create_file_node(t_token *file_token, \
								t_ast_node *redir_node)
{
	t_ast_node	*file_node;

	file_node = create_ast_node(AST_COMMAND, file_token->value);
	if (!file_node)
	{
		free_ast(redir_node);
		return (NULL);
	}
	return (file_node);
}

static t_ast_node	*setup_redirection_nodes(t_token *current,
										t_token *file_token,
										t_ast_node *result)
{
	t_ast_node	*redir_node;
	t_ast_node	*file_node;

	redir_node = create_redirection_node(current->type, file_token->value);
	if (!redir_node)
		return (NULL);
	if (current->type != TOKEN_HEREDOC)
	{
		file_node = create_file_node(file_token, redir_node);
		if (!file_node)
			return (NULL);
		redir_node->right = file_node;
	}
	redir_node->left = result;
	return (redir_node);
}

/**
 * @brief Processes redirection token and creates nodes
 * @param current Current token
 * @param result Current result tree
 * @return Updated result tree or NULL on failure
 */
t_ast_node	*process_redirection(t_token *current, t_ast_node *result)
{
	t_token		*file_token;

	file_token = current->next;
	if (!validate_file_token(file_token))
		return (NULL);
	if (current->type == TOKEN_REDIRECT_IN)
		check_input_file(file_token->value);
	return (setup_redirection_nodes(current, file_token, result));
}
