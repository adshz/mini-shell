/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_processor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:34:31 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:35:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* Redirection Token Processing
 *
 * This file processes redirection tokens in commands
 *
 **/
#include "parser/parser.h"

// Process redirection tokens in a command
bool	ft_process_redirection(t_shell *shell, t_io_node **io_lst)
{
	t_token_type	redir_token_type;
	t_io_node		*tmp_io_node;

	if (shell->parse_err.type)
		return (false);
	while (shell->curr_token && ft_is_redirection(shell->curr_token->type))
	{
		redir_token_type = shell->curr_token->type;
		ft_get_next_token(shell);
		if (!shell->curr_token)
			return (set_parse_err(shell, E_SYNTAX), false);
		if (shell->curr_token->type != TOKEN_IDENTIFIER)
			return (set_parse_err(shell, E_SYNTAX), false);
		tmp_io_node = ft_create_io_node(redir_token_type, \
								shell->curr_token->value);
		if (!tmp_io_node)
			return (set_parse_err(shell, E_MEM), false);
		append_io_node(io_lst, tmp_io_node);
		ft_get_next_token(shell);
	}
	return (true);
}

// Check if token is a redirection
bool	ft_is_redirection(t_token_type type)
{
	if (type == TOKEN_LESS || type == TOKEN_GREAT || type == TOKEN_DLESS \
		|| type == TOKEN_DGREAT)
		return (true);
	return (false);
}

// Convert token type to IO type
t_io_type	convert_to_io_type(t_token_type type)
{
	if (type == TOKEN_LESS)
		return (IO_IN);
	if (type == TOKEN_GREAT)
		return (IO_OUT);
	if (type == TOKEN_DLESS)
		return (IO_HEREDOC);
	return (IO_APPEND);
}
