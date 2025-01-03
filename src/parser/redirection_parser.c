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
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(( char *)filename, STDERR_FILENO);
		ft_putendl_fd(": No such file or directory", STDERR_FILENO);
		return (0);
	}
	if (access(filename, R_OK) == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(( char *)filename, STDERR_FILENO);
		ft_putendl_fd(": Permission denied", STDERR_FILENO);
		return (0);
	}
	return (1);
}

static void print_indent(int level)
{
	for (int i = 0; i < level; i++)
		ft_putstr_fd("  ", STDERR_FILENO);
}

static void print_ast_structure(t_ast_node *node, int level)
{
	if (!node)
		return;

	print_indent(level);
	ft_putstr_fd("Node Type: ", STDERR_FILENO);
	switch(node->type) {
		case AST_COMMAND:
			ft_putstr_fd("COMMAND", STDERR_FILENO);
			if (node->value)
			{
				ft_putstr_fd(" (", STDERR_FILENO);
				ft_putstr_fd(node->value, STDERR_FILENO);
				ft_putstr_fd(")", STDERR_FILENO);
			}
			break;
		case AST_REDIR_IN:
			ft_putstr_fd("REDIR_IN (<)", STDERR_FILENO);
			break;
		case AST_REDIR_OUT:
			ft_putstr_fd("REDIR_OUT (>)", STDERR_FILENO);
			break;
		case AST_REDIR_APPEND:
			ft_putstr_fd("APPEND (>>)", STDERR_FILENO);
			break;
		case AST_HEREDOC:
			ft_putstr_fd("HEREDOC (<<)", STDERR_FILENO);
			if (node->value)
			{
				ft_putstr_fd(" delimiter: ", STDERR_FILENO);
				ft_putstr_fd(node->value, STDERR_FILENO);
			}
			break;
		default:
			ft_putstr_fd("UNKNOWN", STDERR_FILENO);
	}
	ft_putstr_fd("\n", STDERR_FILENO);

	if (node->left)
	{
		print_indent(level + 1);
		ft_putstr_fd("Left: ", STDERR_FILENO);
		print_ast_structure(node->left, level + 1);
	}
	if (node->right)
	{
		print_indent(level + 1);
		ft_putstr_fd("Right: ", STDERR_FILENO);
		print_ast_structure(node->right, level + 1);
	}
}

t_ast_node	*handle_redirection(t_ast_node *node, t_token **tokens)
{
	t_token *current = *tokens;
	t_ast_node *result = node;
	t_token_type prev_type = TOKEN_EOF;

	ft_putstr_fd("\n=== REDIRECTION PARSER START ===\n", STDERR_FILENO);
	ft_putstr_fd("Initial node: ", STDERR_FILENO);
	if (result)
	{
		ft_putstr_fd("Type: ", STDERR_FILENO);
		switch(result->type) {
			case AST_COMMAND: ft_putstr_fd("COMMAND", STDERR_FILENO); break;
			case AST_REDIR_IN: ft_putstr_fd("REDIR_IN", STDERR_FILENO); break;
			case AST_REDIR_OUT: ft_putstr_fd("REDIR_OUT", STDERR_FILENO); break;
			case AST_REDIR_APPEND: ft_putstr_fd("APPEND", STDERR_FILENO); break;
			default: ft_putstr_fd("UNKNOWN", STDERR_FILENO);
		}
		if (result->value)
		{
			ft_putstr_fd(", Value: ", STDERR_FILENO);
			ft_putstr_fd(result->value, STDERR_FILENO);
		}
		ft_putstr_fd("\n", STDERR_FILENO);
	}
	else
		ft_putstr_fd("NULL\n", STDERR_FILENO);

	while (current)
	{
		ft_putstr_fd("\nCurrent token: ", STDERR_FILENO);
		ft_putstr_fd("Type: ", STDERR_FILENO);
		switch(current->type) {
			case TOKEN_WORD: ft_putstr_fd("WORD", STDERR_FILENO); break;
			case TOKEN_REDIRECT_IN: ft_putstr_fd("REDIRECT_IN", STDERR_FILENO); break;
			case TOKEN_REDIRECT_OUT: ft_putstr_fd("REDIRECT_OUT", STDERR_FILENO); break;
			case TOKEN_APPEND: ft_putstr_fd("APPEND", STDERR_FILENO); break;
			default: ft_putstr_fd("OTHER", STDERR_FILENO);
		}
		ft_putstr_fd(", Value: ", STDERR_FILENO);
		ft_putstr_fd(current->value, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);

		// Handle command tokens
		if (current->type == TOKEN_WORD)
		{
			// Skip if this is a filename for a redirection
			if (current->next && is_redirection_token(current->next->type))
			{
				// This is a command followed by a redirection, don't skip
				ft_putstr_fd("Found command before redirection: ", STDERR_FILENO);
				ft_putstr_fd(current->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}
			else if (is_redirection_token(prev_type))
			{
				// This is a filename after a redirection, skip it
				ft_putstr_fd("Skipping filename after redirection: ", STDERR_FILENO);
				ft_putstr_fd(current->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
				prev_type = current->type;
				current = current->next;
				*tokens = current;
				continue;
			}

			ft_putstr_fd("Creating command node for: ", STDERR_FILENO);
			ft_putstr_fd(current->value, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);

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
				ft_putstr_fd("Setting command as left child of redirection\n", STDERR_FILENO);
				// Find the deepest left node to attach the command
				t_ast_node *current_node = result;
				while (current_node->left)
					current_node = current_node->left;
				current_node->left = cmd_node;
			}
			else
			{
				ft_putstr_fd("Setting command as root\n", STDERR_FILENO);
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
			
			ft_putstr_fd("Creating redirection node for: ", STDERR_FILENO);
			ft_putstr_fd(current->value, STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			
			// Move to the file name token
			t_token *file_token = current->next;
			if (!file_token || file_token->type != TOKEN_WORD)
			{
				ft_putendl_fd("minishell: syntax error near unexpected token `newline'", STDERR_FILENO);
				return (NULL);
			}

			// For input redirection, check if file exists and is readable
			if (current->type == TOKEN_REDIRECT_IN)
			{
				ft_putstr_fd("Checking input file: ", STDERR_FILENO);
				ft_putstr_fd(file_token->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
				if (!check_input_file(file_token->value))
					return (NULL);
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
				ft_putstr_fd("Starting heredoc mode with delimiter: ", STDERR_FILENO);
				ft_putstr_fd(file_token->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
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

				ft_putstr_fd("Created file node: ", STDERR_FILENO);
				ft_putstr_fd(file_token->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}
			else
			{
				ft_putstr_fd("Using heredoc delimiter: ", STDERR_FILENO);
				ft_putstr_fd(file_token->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}

			// Set up redirection node
			ft_putstr_fd("Setting up redirection node structure:\n", STDERR_FILENO);
			ft_putstr_fd("- Left child: ", STDERR_FILENO);
			if (result)
			{
				if (result->value)
					ft_putstr_fd(result->value, STDERR_FILENO);
				else
					ft_putstr_fd("NULL", STDERR_FILENO);
			}
			else
				ft_putstr_fd("NULL", STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
			if (file_node)
			{
				ft_putstr_fd("- Right child (file): ", STDERR_FILENO);
				ft_putstr_fd(file_node->value, STDERR_FILENO);
				ft_putstr_fd("\n", STDERR_FILENO);
			}

			// Always preserve the command node as the left child
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

	ft_putstr_fd("\nFinal AST structure:\n", STDERR_FILENO);
	print_ast_structure(result, 0);
	ft_putstr_fd("=== REDIRECTION PARSER END ===\n\n", STDERR_FILENO);

	return result;
} 
