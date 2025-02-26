/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:49:40 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:32:22 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser/parser.h"
#include "lexer/lexer.h"
#include "shell.h"

static void	init_redir_parse(t_redir_parse *parse, t_ast_node *left,
	t_token **tokens, t_shell *shell)
{
	parse->current = *tokens;
	parse->result_left_node = left;
	parse->prev_type = TOKEN_EOF;
	parse->shell = shell;
}

/**
 * @brief Parses a redirection construct and builds its AST
 * @param left Left subtree to attach
 * @param tokens Token stream
 * @param shell Shell instance
 * @return Complete redirection AST or NULL on failure
 * 
 * Builds an AST for a redirection construct, handling both the redirection
 * operator and its target (file or heredoc delimiter).
 */
t_ast_node	*parse_redirection_construct(t_ast_node *left, t_token **tokens,
	t_shell *shell)
{
	t_redir_parse	parse;

	init_redir_parse(&parse, left, tokens, shell);
	while (parse.current)
	{
		if (parse.current->type != TOKEN_WORD
			&& !is_redirection_token(parse.current->type))
			break ;
		if (!process_redir_token(&parse, tokens))
			return (NULL);
	}
	return (parse.result_left_node);
}
