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
#include "parser.h"

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
	ft_putstr_fd("\nDEBUG [validate_file_token]: Validating file token: [", STDERR_FILENO);
	if (file_token)
	{
		ft_putstr_fd(file_token->value, STDERR_FILENO);
		ft_putstr_fd("] of type ", STDERR_FILENO);
		ft_putnbr_fd(file_token->type, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("NULL]\n", STDERR_FILENO);

	if (!file_token || file_token->type != TOKEN_WORD)
	{
		ft_putstr_fd("DEBUG [validate_file_token]: Invalid file token\n", STDERR_FILENO);
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
		return (false);
	}
	ft_putstr_fd("DEBUG [validate_file_token]: File token is valid\n", STDERR_FILENO);
	return (true);
}

static t_ast_node	*create_file_node(t_token *file_token, t_ast_node *redir_node)
{
	t_ast_node	*file_node;

	ft_putstr_fd("\nDEBUG [create_file_node]: Creating file node for: [", STDERR_FILENO);
	ft_putstr_fd(file_token->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	file_node = create_ast_node(AST_COMMAND, file_token->value);
	if (!file_node)
	{
		ft_putstr_fd("DEBUG [create_file_node]: Failed to create file node\n", STDERR_FILENO);
		free_ast(redir_node);
		return (NULL);
	}
	ft_putstr_fd("DEBUG [create_file_node]: Successfully created file node\n", STDERR_FILENO);
	return (file_node);
}

static t_ast_node	*setup_redirection_nodes(t_token *current,
										t_token *file_token,
										t_ast_node *result)
{
	t_ast_node	*redir_node;
	t_ast_node	*file_node;

	ft_putstr_fd("\nDEBUG [setup_redirection_nodes]: Setting up redirection for token: [", STDERR_FILENO);
	ft_putstr_fd(current->value, STDERR_FILENO);
	ft_putstr_fd("] with file: [", STDERR_FILENO);
	ft_putstr_fd(file_token->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	redir_node = create_redirection_node(current->type, file_token->value);
	if (!redir_node)
	{
		ft_putstr_fd("DEBUG [setup_redirection_nodes]: Failed to create redirection node\n", STDERR_FILENO);
		return (NULL);
	}
	if (current->type != TOKEN_HEREDOC)
	{
		file_node = create_file_node(file_token, redir_node);
		if (!file_node)
		{
			ft_putstr_fd("DEBUG [setup_redirection_nodes]: Failed to create file node\n", STDERR_FILENO);
			return (NULL);
		}
		redir_node->right = file_node;
	}
	redir_node->left = result;
	ft_putstr_fd("DEBUG [setup_redirection_nodes]: Successfully set up redirection nodes\n", STDERR_FILENO);
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

	ft_putstr_fd("\nDEBUG [process_redirection]: Processing redirection token: [", STDERR_FILENO);
	ft_putstr_fd(current->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	file_token = current->next;
	if (!validate_file_token(file_token))
		return (NULL);

	if (current->type == TOKEN_REDIRECT_IN)
	{
		ft_putstr_fd("DEBUG [process_redirection]: Checking input file access\n", STDERR_FILENO);
		check_input_file(file_token->value);
	}

	ft_putstr_fd("DEBUG [process_redirection]: Setting up redirection structure\n", STDERR_FILENO);
	return (setup_redirection_nodes(current, file_token, result));
}
