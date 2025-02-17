/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:10:40 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 21:26:16 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

t_token	*find_last_word_token(t_token *start)
{
	t_token	*cmd_token;
	t_token	*last_word;
	int		is_redir_arg;

	cmd_token = start;
	last_word = NULL;
	is_redir_arg = 0;
	while (cmd_token)
	{
		if (is_redirection_token(cmd_token->type))
		{
			is_redir_arg = 1;
			cmd_token = cmd_token->next;
			continue ;
		}
		if (cmd_token->type == TOKEN_WORD)
		{
			if (!is_redir_arg)
				last_word = cmd_token;
			is_redir_arg = 0;
		}
		cmd_token = cmd_token->next;
	}
	return (last_word);
}

static t_ast_node	*cleanup_heredoc_nodes(t_ast_node *first_redir,
	t_ast_node *command_node)
{
	if (first_redir)
		free_ast(first_redir);
	if (command_node)
		free_ast(command_node);
	return (NULL);
}

static t_ast_node	*create_heredoc_chain(t_token **tokens,
	t_ast_node *command_node, t_shell *shell)
{
	t_token		*current;
	t_token		*delimiter;
	t_ast_node	*first_redir;
	int			count;

	current = *tokens;
	first_redir = NULL;
	count = 0;

	// First pass: Count heredocs and validate delimiters
	t_token *check = current;
	while (check && check->type == TOKEN_HEREDOC)
	{
		count++;
		if (!check->next || check->next->type != TOKEN_WORD)
		{
			ft_putstr_fd("Error: Missing or invalid delimiter\n", STDERR_FILENO);
			shell->exit_status = 258;
			return (cleanup_heredoc_nodes(first_redir, command_node));
		}
		check = check->next->next;
	}
	t_token *target = current;
	int current_count = 1;
	while (current_count <= count)
	{
		delimiter = target->next;
		t_ast_node *new_redir = create_redirection_node(TOKEN_HEREDOC,
			delimiter->value);
		if (!new_redir)
		{
			ft_putstr_fd("Failed to create redirection node\n", STDERR_FILENO);
			return (cleanup_heredoc_nodes(first_redir, command_node));
		}
		if (!first_redir)
		{
			new_redir->left = command_node;
			first_redir = new_redir;
		}
		else
		{
			new_redir->left = first_redir;
			first_redir = new_redir;
		}
		target = target->next->next;
		current_count++;
	}
	while (current && current->type == TOKEN_HEREDOC)
	{
		current = current->next->next;
	}
	*tokens = current;
	return (first_redir);
}

t_ast_node	*handle_heredoc_command(t_token **tokens, t_shell *shell)
{
	t_ast_node	*command_node;
	t_ast_node	*heredoc_chain;

	command_node = create_default_heredoc_command();
	if (!command_node)
	{
		ft_putstr_fd("Failed to create default heredoc command\n", STDERR_FILENO);
		return (NULL);
	}
	heredoc_chain = create_heredoc_chain(tokens, command_node, shell);
	if (!heredoc_chain)
	{
		ft_putstr_fd("Failed to create heredoc chain\n", STDERR_FILENO);
		free_ast(command_node);
		return (NULL);
	}
	return (heredoc_chain);
}
