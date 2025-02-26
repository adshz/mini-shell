/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:22:08 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:23:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Parser State Management
 *
 * The file manages the state and handles errors
 *
 */
// Advance to next token during parsing
void	ft_get_next_token(t_shell *shell)
{
	shell->curr_token = shell->curr_token->next;
}

void	set_parse_err(t_shell *shell, t_parse_err_type type)
{
	shell->parse_err.type = type;
}

void	parse_err_handler(t_shell *shell)
{
	t_parse_err_type	err_type;
	t_token_type		token_type;
	char				**types;

	types = (char *[]){"TOKEN_IDENTIFIER", "<", ">", "<<", ">>", \
		"|", "(", ")", "&&", "||", "newline"};
	err_type = shell->parse_err.type;
	(void)token_type;
	(void)types;
	if (type)
	{
		if (type == E_SYNTAX)
		{
			if (!shell->curr_token)
				token_type = TOKEN_NEWLINE;
			else
				token_type = shell->curr_token->type;
			ft_putstr("minishell: syntax error near unexpected token '", 2);
			ft_putstr(types[token_type], 2);
			shell->exit_status = 258;
		}
		ft_cleanup_ast(shell, &(shell->ast));
		ft_bzero(&shell->parse_err, sizeof(t_parse_err));
	}
}
