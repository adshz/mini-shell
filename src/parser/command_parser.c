/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:43:36 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 12:43:45 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser.h"
#include "lexer/lexer.h"
#include "shell.h"

/**
 * @brief Main command parsing function
 * Delegates to appropriate handlers based on token type
 */
t_ast_node *parse_command(t_token **tokens, t_shell *shell)
{
	t_token *start;
	t_ast_node *node;
	int arg_count;

	if (!tokens || !*tokens)
		return (NULL);
	start = *tokens;
	if (start->type == TOKEN_HEREDOC)
		return (handle_heredoc_command(tokens, shell));
	node = process_initial_redirection(tokens, shell);
	if (node)
		return (node);
	arg_count = count_command_args(start);
	if (arg_count == 0)
		return (NULL);
	node = create_regular_command(tokens, start, arg_count);
	if (!node)
		return (NULL);
	return (process_final_redirection(node, tokens, shell));
}