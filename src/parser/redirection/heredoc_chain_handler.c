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
	new_redir = create_redirection_node(TOKEN_HEREDOC, hc->delimiter->value);
	if (!new_redir)
		return (false);
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
	if (first_redir)
		free_ast(first_redir);
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
	}
	(hc.last_redir)->left = command_node;
	*(hc.tokens) = hc.current;
	return (hc.first_redir);
}
