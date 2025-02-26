/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_heredoc_handler.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:43:39 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 17:44:48 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"

static void	advance_heredoc_token_stream(t_redir_token *rt)
{
	rt->prev_type = (*rt->current)->type;
	if (rt->next_token && rt->next_token->next)
	{
		*rt->tokens = rt->next_token->next;
		*rt->current = *rt->tokens;
	}
	else
	{
		*rt->tokens = NULL;
		*rt->current = NULL;
	}
}

/*
 * @brief Handles the heredoc token
 * 
 * This function handles the heredoc token.
 * It finds the command after the heredoc token and processes the redirection.
 * 
 * @note
 * as for the case of `<<hello > file.txt`, 
 * tokens are 
 * - << (TOKEN_HEREDOC)
 * - hello (TOKEN_WORD)
 * - > (TOKEN_REDIRECT_OUT)
 * - file.txt (TOKEN_WORD)
 * 
 * double pointer `current` for nevigating
 * *current -> "<<" -> "hello" -> ">" -> "out1"
 * 
 * when we get into this function, current is "<<"
 * 
 * rt->next_token = "hello"
 * rt->result_left_node = NULL
 * rt->output_node = 
 * rt->prev_type = TOKEN_EOF
 * rt->shell = shell
 * 
 * process_redirection() -> setup_redirection_nodes() 
 * -> create_redirection_node() -> file_node
 * 
 *
 * When we're at <<:
 * rt->next_token is "hello"
 * rt->next_token->next is ">" so we will advance 
 * *rt->current will point to ">"
 *  by doing this, we skip `<<` and `hello`
*/
static bool	validate_heredoc_word_token(t_redir_token *rt, \
									t_token *delimiter_token)
{
	if (!delimiter_token || delimiter_token->type != TOKEN_WORD)
	{
		rt->shell->exit_status = 258;
		return (false);
	}
	return (true);
}

/**
 * @brief Set up the left node for the heredoc token
 * 
 * This function sets up the left node for the heredoc token.
 * 
 * @note
 * as for the case of `<<hello > file.txt`, after lexing, tokens are 
 * - << (its types is TOKEN_HEREDOC)
 * - hello (TOKEN_WORD)
 * - > (TOKEN_REDIRECT_OUT)
 * - file.txt (TOKEN_WORD)
 * 
 * 
 */
static bool	setup_heredoc_left_node(t_redir_token *rt, t_token *delimiter_token)
{
	if (!rt->result_left_node)
	{
		if (delimiter_token && delimiter_token->next && \
			delimiter_token->next->type == TOKEN_WORD)
			rt->result_left_node = \
				create_command_from_token(delimiter_token->next);
		if (!rt->result_left_node && rt->shell->exit_status != 258)
		{
			rt->shell->exit_status = 258;
			return (false);
		}
	}
	return (true);
}

static bool	create_heredoc_ast_node(t_redir_token *rt)
{
	rt->output_node = process_redirection(*rt->current, rt->result_left_node);
	if (!rt->output_node)
	{
		rt->shell->exit_status = 258;
		return (false);
	}
	return (true);
}

t_ast_node	*handle_heredoc_token(t_redir_token *rt)
{
	t_token	*delimiter_token;

	delimiter_token = NULL;
	delimiter_token = (*rt->current)->next;
	rt->next_token = delimiter_token;
	if (!validate_heredoc_word_token(rt, delimiter_token))
		return (NULL);
	if (!setup_heredoc_left_node(rt, delimiter_token))
		return (NULL);
	if (!create_heredoc_ast_node(rt))
		return (NULL);
	advance_heredoc_token_stream(rt);
	return (rt->output_node);
}
