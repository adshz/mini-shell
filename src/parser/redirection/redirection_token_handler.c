/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_token_handler.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:41:37 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 17:42:05 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/**
 * @brief Handles the regular redirection token
 * 
 * This function handles the regular redirection token.
 * It processes the redirection and returns the output node.
 * 
 * @note
 * as for the case of `echo hello> file.txt`, 
 * tokens are 
 * - echo (TOKEN_WORD)
 * - hello (TOKEN_WORD)
 * - > (TOKEN_REDIRECT_OUT)
 * - file.txt (TOKEN_WORD)
 * 
 * *rt->current points to regular redirection token
 * - > (TOKEN_REDIRECT_OUT)
 * - >> (TOKEN_REDIRECT_APPEND)
 * - < (TOKEN_REDIRECT_IN)
 * 
 * case `echo hello > file.txt`
 * rt->result_left_node is the command before the redirection token
 * which is `echo hello`
 * 
 * rt->output_node is the output node of the redirection
 * which is `file.txt`
 * 
 * rt->prev_type is the previous token type
 * which is TOKEN_EOF
 * 
 * rt->shell is the shell instance
 * 
 * First of all, t_token **current, t_token **tokens are double pointers
 * tokens tracks the overall token stream position
 * current is used for local navigation within the current parsing context
 * example: echo hello > file.txt
 * When we are in the function call, our 
 * - *rt->current points to ">" (Token 3) because it's following parse->current
 * - *rt->tokens points to ">" (Token 3) because it didn't move, points
 * to the same as *rt->current
 * 
 * go through rt->output_node = process_redirection();
 * - rt->prev_type  // Store TOKEN_REDIRECT_OUT
* // Skip both ">" and "file.txt"
* - *rt->tokens = (*rt->current)->next->next;  // Now points to Token 5 (NULL)
* // Sync current with tokens
* - *rt->current = *rt->tokens; // Now points to Token 5 (NULL)
 */
static t_ast_node	*handle_regular_redirection(t_redir_token *rt)
{
	rt->output_node = process_redirection(*rt->current, rt->result_left_node);
	if (!rt->output_node)
	{
		rt->shell->exit_status = 258;
		return (NULL);
	}
	rt->prev_type = (*rt->current)->type;
	*rt->tokens = (*rt->current)->next->next;
	*rt->current = *rt->tokens;
	return (rt->output_node);
}

static void	init_redir_token(t_redir_token *rt, t_redir_parse *parse, \
							t_token **tokens)
{
	rt->current = &parse->current;
	rt->tokens = tokens;
	rt->next_token = NULL;
	rt->result_left_node = parse->result_left_node;
	rt->output_node = NULL;
	rt->prev_type = parse->prev_type;
	rt->shell = parse->shell;
}

/**
 * @brief Handles the redirection token
 * 
 * This function handles the redirection token.
 * It either handles the heredoc token or the regular redirection token.
 * @note
 * well we have 2 kinds of redirection tokens:
 * - heredoc token (e.g. `<<`)
 * - regular redirection token (e.g. `>`, `>>`, `<`)

 */
static t_ast_node	*handle_redirection_token(t_redir_token *rt)
{
	if ((*rt->current)->type == TOKEN_HEREDOC)
		return (handle_heredoc_token(rt));
	return (handle_regular_redirection(rt));
}

/**
 * @brief Processes the redirection token
 * 
 * This function processes the redirection token.
 * It either handles the word token or the redirection token.
 * @note
 * there are if -else, so we first handle word token
 * then we handle the redirection token 
 * as its parent function parse_redirection_construct() has a while loop
 * iterate through each token in the tokens list
 */
t_ast_node	*process_redir_token(t_redir_parse *parse, t_token **tokens)
{
	t_word_token	wt;
	t_redir_token	rt;

	if (parse->current->type == TOKEN_WORD)
	{
		init_word_token(&wt, parse, tokens);
		parse->result_left_node = handle_word_token(&wt);
	}
	else if (is_redirection_token(parse->current->type))
	{
		init_redir_token(&rt, parse, tokens);
		parse->result_left_node = handle_redirection_token(&rt);
	}
	if (!parse->result_left_node)
	{
		parse->shell->exit_status = 258;
		return (NULL);
	}
	return (parse->result_left_node);
}
