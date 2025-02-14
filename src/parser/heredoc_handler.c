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

t_ast_node	*create_default_heredoc_command(void)
{
	t_token		*cat_token;
	t_ast_node	*node;

	cat_token = create_token(TOKEN_WORD, "cat");
	if (!cat_token)
		return (NULL);
	node = init_command_node(cat_token, 1);
	if (!node)
	{
		free_tokens(cat_token);
		return (NULL);
	}
	if (!fill_args(node->args, cat_token, 1))
	{
		free_tokens(cat_token);
		free_ast(node);
		return (NULL);
	}
	free_tokens(cat_token);
	return (node);
}

t_ast_node	*handle_heredoc_command(t_token **tokens, t_shell *shell)
{
	t_token		*heredoc_token;
	t_token		*delimiter_token;
	t_ast_node	*node;
	t_ast_node	*redir_node;

	heredoc_token = *tokens;
	delimiter_token = heredoc_token->next;
	if (!delimiter_token || delimiter_token->type != TOKEN_WORD)
	{
		ft_putendl_fd("minishell: syntax error near unexpected token `newline'",
			STDERR_FILENO);
		shell->exit_status = 258;
		return (NULL);
	}
	node = create_default_heredoc_command();
	if (!node)
		return (NULL);
	redir_node = create_redirection_node(TOKEN_HEREDOC, delimiter_token->value);
	if (!redir_node)
	{
		free_ast(node);
		return (NULL);
	}
	redir_node->left = node;
	*tokens = delimiter_token->next;
	return (redir_node);
}
