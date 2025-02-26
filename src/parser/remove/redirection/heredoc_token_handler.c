/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_token_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 17:42:33 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 17:42:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

void	init_word_token(t_word_token *wt, t_redir_parse *parse,
	t_token **tokens)
{
	wt->current = &parse->current;
	wt->tokens = tokens;
	wt->prev_type = &parse->prev_type;
	wt->result = parse->result_left_node;
	wt->shell = parse->shell;
	wt->cmd_node = NULL;
}

void	update_token_positions(t_word_token *wt)
{
	*wt->prev_type = (*wt->current)->type;
	*wt->current = (*wt->current)->next;
	*wt->tokens = *wt->current;
}

t_ast_node	*create_and_attach_command(t_word_token *wt)
{
	wt->cmd_node = create_command_from_token(*wt->current);
	if (!wt->cmd_node)
	{
		wt->shell->exit_status = 258;
		return (NULL);
	}
	wt->result = attach_command_node(wt->result, wt->cmd_node);
	update_token_positions(wt);
	return (wt->result);
}

/**
 * @brief Checks if the current word token is part of a redirection
 * 
 * This function checks if the next token is a redirection token or if the
 * previous token is a redirection token. If so, it updates the token positions
 * and returns the result. Otherwise, it returns NULL.
 * @note
 * as for the case of `echo hello > file.txt`, tokens are 
 * echo (TOKEN_WORD)
 * hello (TOKEN_WORD)
 * > (TOKEN_REDIRECT_OUT)
 * file.txt (TOKEN_WORD)
 * the flow will be like this:
 * When current = "echo"
 * if ((*wt->current)->next && 
 *      is_redirection_token((*wt->current)->next->type))
 *     // next is "hello", not a redirection, so continue
 * if (is_redirection_token(*wt->prev_type))
 *   // prev is none/EOF, not a redirection, so continue
 * return (NULL);  // Process "echo" as a normal command

 * 4. the result is the command node `echo`
*  5. the result is attached to the command node `echo`
 *   (*wt->current) is the word token `echo`
 *   (*wt->current)->next is the redirection token `>`
 *   (*wt->prev_type) is the previous token type which is TOKEN_EOF (default)
 *
*  
 * 5. the token positions are updated
 * 6. the result is returned
 */
t_ast_node	*is_word_part_of_redirection(t_word_token *wt)
{
	if ((*wt->current)->next && \
		is_redirection_token((*wt->current)->next->type))
	{
		update_token_positions(wt);
		return (wt->result);
	}
	if (is_redirection_token(*wt->prev_type))
	{
		update_token_positions(wt);
		return (wt->result);
	}
	return (NULL);
}

t_ast_node	*handle_word_token(t_word_token *wt)
{
	t_ast_node		*redir_result;

	redir_result = is_word_part_of_redirection(wt);
	if (redir_result)
		return (redir_result);
	return (create_and_attach_command(wt));
}
