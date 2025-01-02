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

static t_ast_node	*create_redir_node(t_token_type type, const char *target)
{
	t_ast_node	*redir_node;
	t_ast_node	*target_node;

	redir_node = create_ast_node(get_ast_type(type), NULL);
	if (!redir_node)
		return (NULL);
	target_node = create_ast_node(AST_COMMAND, (char *)target);
	if (!target_node)
	{
		free_ast(redir_node);
		return (NULL);
	}
	redir_node->right = target_node;
	return (redir_node);
}

t_ast_node	*handle_redirection(t_ast_node *left, t_token **tokens)
{
	t_token_type	redir_type;
	t_ast_node		*redir_node;
	t_token			*current;
	char			**new_args;
	int				i;
	int				arg_count;

	redir_type = (*tokens)->type;
	*tokens = (*tokens)->next;
	current = *tokens;
	if (!current || current->type != TOKEN_WORD)
	{
		free_ast(left);
		return (NULL);
	}

	// Create redirection node
	redir_node = create_redir_node(redir_type, current->value);
	if (!redir_node)
	{
		free_ast(left);
		return (NULL);
	}

	// Move past the redirection target
	*tokens = current->next;

	// If there are more tokens and we have a left node (command)
	if (left && left->args)
	{
		// Count remaining arguments
		current = *tokens;
		arg_count = 0;
		while (current && current->type == TOKEN_WORD)
		{
			arg_count++;
			current = current->next;
		}

		// Create new args array with additional space for remaining tokens
		i = 0;
		while (left->args[i])
			i++;
		new_args = malloc(sizeof(char *) * (i + arg_count + 1));
		if (!new_args)
		{
			free_ast(redir_node);
			free_ast(left);
			return (NULL);
		}

		// Copy existing arguments
		i = 0;
		while (left->args[i])
		{
			new_args[i] = ft_strdup(left->args[i]);
			if (!new_args[i])
			{
				while (i > 0)
					free(new_args[--i]);
				free(new_args);
				free_ast(redir_node);
				free_ast(left);
				return (NULL);
			}
			i++;
		}

		// Add remaining arguments
		current = *tokens;
		while (current && current->type == TOKEN_WORD)
		{
			new_args[i] = ft_strdup(current->value);
			if (!new_args[i])
			{
				while (i > 0)
					free(new_args[--i]);
				free(new_args);
				free_ast(redir_node);
				free_ast(left);
				return (NULL);
			}
			i++;
			current = current->next;
		}
		new_args[i] = NULL;

		// Update tokens pointer
		*tokens = current;

		// Free old args and update with new ones
		i = 0;
		while (left->args[i])
			free(left->args[i++]);
		free(left->args);
		left->args = new_args;
	}

	redir_node->left = left;
	return (redir_node);
} 
