/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:10:40 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 23:17:09 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

static int	count_heredoc_tokens(t_token *tokens, t_shell *shell)
{
	t_token	*check;
	int		count;

	check = tokens;
	count = 0;
	while (check && check->type == TOKEN_HEREDOC)
	{
		count++;
		if (!check->next || check->next->type != TOKEN_WORD)
		{
			shell->exit_status = 258;
			return (-1);
		}
		check = check->next->next;
	}
	return (count);
}

static void	link_heredoc_node(t_ast_node **first_redir,
	t_ast_node *new_redir, t_ast_node *command_node)
{
	if (!*first_redir)
	{
		new_redir->left = command_node;
		*first_redir = new_redir;
	}
	else
	{
		new_redir->left = *first_redir;
		*first_redir = new_redir;
	}
}

static t_ast_node	*create_heredoc_node(t_token *token,
	t_ast_node **first_redir, t_ast_node *command_node)
{
	t_ast_node	*new_redir;

	new_redir = create_redirection_node(TOKEN_HEREDOC, token->next->value);
	if (!new_redir)
	{
		if (*first_redir)
			free_ast(*first_redir);
		if (command_node)
			free_ast(command_node);
		return (NULL);
	}
	link_heredoc_node(first_redir, new_redir, command_node);
	return (new_redir);
}

static t_ast_node	*build_heredoc_chain(t_token **tokens,
	t_ast_node *command_node, int count)
{
	t_token		*current;
	t_ast_node	*first_redir;
	int			i;

	current = *tokens;
	first_redir = NULL;
	i = 0;
	while (i < count)
	{
		if (!create_heredoc_node(current, &first_redir, command_node))
			return (NULL);
		current = current->next->next;
		i++;
	}
	*tokens = current;
	return (first_redir);
}

t_ast_node	*handle_heredoc_command(t_token **tokens, t_shell *shell)
{
	t_ast_node	*command_node;
	t_ast_node	*heredoc_chain;
	int			count;

	command_node = create_default_heredoc_command();
	if (!command_node)
		return (NULL);
	count = count_heredoc_tokens(*tokens, shell);
	if (count == -1)
	{
		free_ast(command_node);
		return (NULL);
	}
	heredoc_chain = build_heredoc_chain(tokens, command_node, count);
	if (!heredoc_chain)
	{
		free_ast(command_node);
		return (NULL);
	}
	return (heredoc_chain);
}
