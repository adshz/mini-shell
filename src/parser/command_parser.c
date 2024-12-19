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

static int	fill_args(char **args, t_token *start, int arg_count)
{
	t_token	*current;
	int		i;

	current = start;
	i = 0;
	while (i < arg_count)
	{
		args[i] = ft_strdup(current->value);
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

	start = *tokens;
	arg_count = count_args(start);
	if (arg_count == 0)
		return (NULL);
	node = create_ast_node(AST_COMMAND, start->value);
	if (!node)
		return (NULL);
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
	*tokens = start;
	while (arg_count--)
		*tokens = (*tokens)->next;
	return (node);
} 