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

	err_type = shell->parse_err.type;
	if (err_type)
	{
		if (err_type == E_SYNTAX)
		{
			if (!shell->curr_token)
				ft_putstr_fd("minishell: syntax error near unexpected token 'newline'\n", 2);
			else
			{
				ft_putstr_fd("minishell: syntax error near unexpected token '", 2);
				if (shell->curr_token->type == TOKEN_IDENTIFIER)
					ft_putstr_fd(shell->curr_token->value, 2);
				else if (shell->curr_token->type == TOKEN_LESS)
					ft_putstr_fd("<", 2);
				else if (shell->curr_token->type == TOKEN_GREAT)
					ft_putstr_fd(">", 2);
				else if (shell->curr_token->type == TOKEN_DLESS)
					ft_putstr_fd("<<", 2);
				else if (shell->curr_token->type == TOKEN_DGREAT)
					ft_putstr_fd(">>", 2);
				else if (shell->curr_token->type == TOKEN_PIPE)
					ft_putstr_fd("|", 2);
				else if (shell->curr_token->type == TOKEN_O_PARENT)
					ft_putstr_fd("(", 2);
				else if (shell->curr_token->type == TOKEN_C_PARENT)
					ft_putstr_fd(")", 2);
				else if (shell->curr_token->type == TOKEN_AND)
					ft_putstr_fd("&&", 2);
				else if (shell->curr_token->type == TOKEN_OR)
					ft_putstr_fd("||", 2);
				ft_putstr_fd("'\n", 2);
			}
			shell->exit_status = 258;
		}
		ft_cleanup_ast(shell, &(shell->ast));
		ft_bzero(&shell->parse_err, sizeof(t_parse_err));
	}
}
