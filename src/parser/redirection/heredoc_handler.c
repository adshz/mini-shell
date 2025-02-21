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

#include "parser/parser.h"
#include "libft.h"

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

t_ast_node	*handle_heredoc_command(t_token **tokens, t_shell *shell)
{
	t_ast_node	*command_node;
	t_ast_node	*heredoc_chain;

	if (shell->heredoc_sigint || g_signal_status == SIG_HEREDOC_INT)
		return (NULL);
	command_node = create_default_heredoc_command();
	if (!command_node)
		return (NULL);
	if (shell->heredoc_sigint || g_signal_status == SIG_HEREDOC_INT)
	{
		free_ast(command_node);
		return (NULL);
	}
	heredoc_chain = create_heredoc_chain(tokens, command_node, shell);
	if (!heredoc_chain)
	{
		free_ast(command_node);
		return (NULL);
	}
	return (heredoc_chain);
}
