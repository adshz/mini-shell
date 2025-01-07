/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"

static int check_input_file(const char *filename)
{
	if (access(filename, F_OK) == -1)
		return (0);
	if (access(filename, R_OK) == -1)
		return (0);
	return (1);
}

t_ast_node	*handle_redirection(t_ast_node *left, t_token **tokens)
{
	t_token *current = *tokens;
	t_ast_node *result = left;
	t_token_type prev_type = TOKEN_EOF;

	while (current)
	{
		// Handle command tokens
		if (current->type == TOKEN_WORD)
		{
			// Skip if this is a filename for a redirection
			if (current->next && is_redirection_token(current->next->type))
			{
				// This is a command followed by a redirection, don't skip
			}
			else if (is_redirection_token(prev_type))
			{
				// This is a filename after a redirection, skip it
				prev_type = current->type;
				current = current->next;
				*tokens = current;
				continue;
			}

			// Create command node
			t_ast_node *cmd_node = create_ast_node(AST_COMMAND, current->value);
			if (!cmd_node)
				return NULL;
			
			cmd_node->args = malloc(sizeof(char *) * 2);
			if (!cmd_node->args)
			{
				free_ast(cmd_node);
				return NULL;
			}
			cmd_node->args[0] = ft_strdup(current->value);
			cmd_node->args[1] = NULL;

			// If we have a redirection node, set the command as its left child
			if (result)
			{
				// Find the deepest left node to attach the command
				t_ast_node *current_node = result;
				while (current_node->left)
					current_node = current_node->left;
				current_node->left = cmd_node;
			}
			else
			{
				result = cmd_node;
			}
			
			prev_type = current->type;
			current = current->next;
			*tokens = current;
			continue;
		}

		// Handle redirection tokens
		if (is_redirection_token(current->type))
		{
			t_ast_node *redir_node;
			
			// Move to the file name token
			t_token *file_token = current->next;
			if (!file_token || file_token->type != TOKEN_WORD)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
				return NULL;
			}

			// For input redirection, check if file exists and is readable
			if (current->type == TOKEN_REDIRECT_IN)
			{
				check_input_file(file_token->value);  // Just check and print error, don't return
			}

			if (current->type == TOKEN_REDIRECT_OUT)
				redir_node = create_ast_node(AST_REDIR_OUT, NULL);
			else if (current->type == TOKEN_REDIRECT_IN)
				redir_node = create_ast_node(AST_REDIR_IN, NULL);
			else if (current->type == TOKEN_APPEND)
				redir_node = create_ast_node(AST_REDIR_APPEND, NULL);
			else if (current->type == TOKEN_HEREDOC)
			{
				redir_node = create_ast_node(AST_HEREDOC, file_token->value);  // Store delimiter in node value
				if (!redir_node)
					return (NULL);
			}
			else
				redir_node = create_ast_node(AST_HEREDOC, NULL);

			if (!redir_node)
				return (NULL);

			// Create file/delimiter node only for non-heredoc redirections
			t_ast_node *file_node = NULL;
			if (current->type != TOKEN_HEREDOC)
			{
				file_node = create_ast_node(AST_COMMAND, file_token->value);
				if (!file_node)
				{
					free_ast(redir_node);
					return (NULL);
				}
			}

			// Set up redirection node
			redir_node->left = result;  // Previous node (command) becomes left child
			if (file_node)
				redir_node->right = file_node;  // File becomes right child (only for non-heredoc)
			result = redir_node;  // This redirection becomes the new root

			// Move to next token
			prev_type = current->type;
			*tokens = file_token->next;
			current = *tokens;
			continue;
		}

		// If we get here, we've found a token we don't handle
		break;
	}

	return result;
} 
