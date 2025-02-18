/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 19:51:24 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/17 23:44:44 by evmouka          ###   ########.fr       */
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
