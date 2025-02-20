/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 heredoc_chain_handler.c							:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/19 18:13:11 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/19 18:13:13 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "parser/parser.h"

static void	init_heredoc_chain(t_heredoc_chain *hc, t_token **tokens, \
							t_ast_node *cmd_node, t_shell *shell)
{
	hc->tokens = tokens;
	hc->current = *tokens;
	hc->delimiter = NULL;
	hc->first_redir = NULL;
	hc->last_redir = NULL;
	hc->cmd_node = cmd_node;
	hc->shell = shell;
	hc->total_heredocs = 0;
	hc->processed_count = 0;
}

static bool	count_heredoc_tokens(t_heredoc_chain *hc)
{
	t_token	*check;

	check = hc->current;
	while (check && check->type == TOKEN_HEREDOC)
	{
		hc->total_heredocs++;
		if (!check->next || check->next->type != TOKEN_WORD)
		{
			hc->shell->exit_status = 258;
			return (false);
		}
		check = check->next->next;
	}
	return (true);
}

static bool	add_heredoc_node(t_heredoc_chain *hc)
{
	t_ast_node	*new_redir;

	hc->delimiter = hc->current->next;
	// Check for signal before allocating
	if (hc->shell->heredoc_sigint || g_signal_status == SIG_HEREDOC_INT)
		return (false);
	new_redir = create_redirection_node(TOKEN_HEREDOC, hc->delimiter->value);
	if (!new_redir)
		return (false);
	// Check for signal right after allocation
	if (hc->shell->heredoc_sigint || g_signal_status == SIG_HEREDOC_INT)
	{
		free_ast(new_redir);  // Free the node if signal interrupted
		return (false);
	}
	if (!hc->first_redir)
	{
		hc->first_redir = new_redir;
		hc->last_redir = new_redir;
	}
	else
	{
		hc->last_redir->left = new_redir;
		hc->last_redir = new_redir;
	}
	hc->current = hc->current->next->next;
	hc->processed_count++;
	return (true);
}

static t_ast_node	*cleanup_heredoc_nodes(t_ast_node *first_redir, \
										t_ast_node *command_node)
{
	t_ast_node *current;
	t_ast_node *next;

	if (first_redir)
	{
		// First detach command_node to avoid double free
		current = first_redir;
		while (current)
		{
			if (current->left == command_node)
			{
				current->left = NULL;
				break;
			}
			current = current->left;
		}

		// Now free the heredoc chain
		current = first_redir;
		while (current)
		{
			next = current->left;
			// Free right child (delimiter node)
			if (current->right)
			{
				if (current->right->value)
					free(current->right->value);
				if (current->right->args)
					ft_free_array(current->right->args);
				if (current->right->original)
					free(current->right->original);
				free(current->right);
			}
			// Free current node
			if (current->value)
				free(current->value);
			if (current->args)
				ft_free_array(current->args);
			if (current->data.content_path)
				free(current->data.content_path);
			if (current->original)
				free(current->original);
			if (current->data.delimiter)  // Now we own this string, so we must free it
				free((char *)current->data.delimiter);
			free(current);
			current = next;
		}
	}
	if (command_node)
		free_ast(command_node);
	return (NULL);
}

t_ast_node	*create_heredoc_chain(t_token **tokens, \
								t_ast_node *command_node, t_shell *shell)
{
	t_heredoc_chain	hc;

	init_heredoc_chain(&hc, tokens, command_node, shell);
	if (!count_heredoc_tokens(&hc))
		return (cleanup_heredoc_nodes(hc.first_redir, hc.cmd_node));
	while (hc.processed_count < hc.total_heredocs)
	{
		if (!add_heredoc_node(&hc))
			return (cleanup_heredoc_nodes(hc.first_redir, hc.cmd_node));
		if (shell->heredoc_sigint || g_signal_status == SIG_HEREDOC_INT)
		{
			shell->heredoc_sigint = true;
			g_signal_status = SIG_NONE;
			return (cleanup_heredoc_nodes(hc.first_redir, hc.cmd_node));
		}
	}
	(hc.last_redir)->left = command_node;
	*(hc.tokens) = hc.current;
	return (hc.first_redir);
}
