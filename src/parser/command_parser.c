/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:59 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:59 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "libft.h"
#include <stdio.h>
static int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

static char	**allocate_args(int arg_count)
{
	char	**args;

	args = malloc(sizeof(char *) * (arg_count + 1));
	if (!args)
		return (NULL);
	args[arg_count] = NULL;
	return (args);
}

int is_variable_string(const char *token)
{
	return (token && token[0] == '$' && token[1] != '\0');
}

static int	fill_args(char **args, t_token *start, int arg_count)
{
	t_token	*current;
	int		i;

	current = start;
	i = 0;
	while (i < arg_count)
	{
		if (is_variable_string(current->value))
		{
			args[i] = ft_strdup(current->value);
		}
		else
		{
			args[i] = ft_strdup(current->value);
		}
		if (!args[i])
			return (0);
		current = current->next;
		i++;
	}
	return (1);
}

static int is_variable_assignment(const char *str)
{
	int i;

	if (!str || !*str)
		return (0);
	
	// First character must be letter or underscore
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	
	// Find equals sign
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	
	// Must have equals sign
	return (str[i] == '=');
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;
	t_token		*start;
	int			arg_count;

	start = *tokens;
	if (!start)
		return (NULL);

	// Check for redirection tokens first
	if (start->type == TOKEN_REDIRECT_IN || start->type == TOKEN_REDIRECT_OUT ||
		start->type == TOKEN_APPEND || start->type == TOKEN_HEREDOC)
	{
		return (handle_redirection(NULL, tokens));
	}

	// Check if this is a variable assignment
	if (is_variable_assignment(start->value))
	{
		node = create_ast_node(AST_VAR_EXPANSION, start->value);
		if (!node)
			return (NULL);
		*tokens = (*tokens)->next;
		return (node);
	}

	arg_count = count_args(start);
	if (arg_count == 0)
		return (NULL);

	node = create_ast_node(AST_COMMAND, start->value);
	if (!node)
		return (NULL);

	node->is_expanded = is_variable_string(start->value);
	if (node->is_expanded)
		node->original = ft_strdup(start->value);

	node->args = allocate_args(arg_count);
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}

	if (!fill_args(node->args, start, arg_count))
	{
		free_ast(node);
		return (NULL);
	}

	// Advance tokens past the command arguments
	while (arg_count > 0)
	{
		*tokens = (*tokens)->next;
		arg_count--;
	}

	// Check for redirection after the command
	if (*tokens && ((*tokens)->type == TOKEN_REDIRECT_IN || 
		(*tokens)->type == TOKEN_REDIRECT_OUT ||
		(*tokens)->type == TOKEN_APPEND || 
		(*tokens)->type == TOKEN_HEREDOC))
	{
		return (handle_redirection(node, tokens));
	}

	return (node);
} 