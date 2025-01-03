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

int is_variable_token(const char *token)
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
		if (is_variable_token(current->value))
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

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;
	int			arg_count;
	t_token		*start;

	ft_putstr_fd("\n=== COMMAND PARSER START ===\n", STDERR_FILENO);
	start = *tokens;
	if (!start)
	{
		ft_putstr_fd("Error: No tokens to parse\n", STDERR_FILENO);
		return (NULL);
	}

	ft_putstr_fd("Parsing command token: ", STDERR_FILENO);
	ft_putstr_fd(start->value, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	// Check for redirection tokens first
	if (start->type == TOKEN_REDIRECT_IN || start->type == TOKEN_REDIRECT_OUT ||
		start->type == TOKEN_APPEND || start->type == TOKEN_HEREDOC)
	{
		ft_putstr_fd("Found redirection token, handling redirection\n", STDERR_FILENO);
		return (handle_redirection(NULL, tokens));
	}

	arg_count = count_args(start);
	if (arg_count == 0)
	{
		ft_putstr_fd("Error: No arguments found\n", STDERR_FILENO);
		return (NULL);
	}

	ft_putstr_fd("Number of arguments found: ", STDERR_FILENO);
	ft_putnbr_fd(arg_count, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	node = create_ast_node(AST_COMMAND, start->value);
	if (!node)
	{
		ft_putstr_fd("Error: Failed to create command node\n", STDERR_FILENO);
		return (NULL);
	}

	ft_putstr_fd("Created command node with value: ", STDERR_FILENO);
	ft_putstr_fd(node->value, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	node->is_expanded = is_variable_token(start->value);
	if (node->is_expanded)
	{
		ft_putstr_fd("Token is a variable, storing original value\n", STDERR_FILENO);
		node->original = ft_strdup(start->value);
	}

	node->args = allocate_args(arg_count);
	if (!node->args)
	{
		ft_putstr_fd("Error: Failed to allocate arguments array\n", STDERR_FILENO);
		free_ast(node);
		return (NULL);
	}

	if (!fill_args(node->args, start, arg_count))
	{
		ft_putstr_fd("Error: Failed to fill arguments array\n", STDERR_FILENO);
		free_ast(node);
		return (NULL);
	}

	ft_putstr_fd("Arguments filled successfully. Command arguments:\n", STDERR_FILENO);
	int i = 0;
	while (node->args[i])
	{
		ft_putstr_fd("  arg[", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd("]: ", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		i++;
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
		ft_putstr_fd("Found redirection after command, handling redirection\n", STDERR_FILENO);
		return (handle_redirection(node, tokens));
	}

	ft_putstr_fd("=== COMMAND PARSER END ===\n\n", STDERR_FILENO);
	return (node);
} 