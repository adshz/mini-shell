/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_validate.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:08:50 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:09:51 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Print error for unclosed quotes
 *
 * @param shell Shell structure for updating exit status
 * @param c The quote character that was not closed
 * @return void
 */
void	error_quote_printer(t_shell *shell, char c)
{
	ft_putstr_fd("minishell: unexpected EOF while looking for matching '", 2);
	ft_putchar_fd(c, 2);
	ft_putchar_fd('\'', 2);
	ft_putchar_fd('\n', 2);
	shell->exit_status = 258;
}

/**
 * @brief Validate the token list for syntax errors
 *
 * @param shell Shell structure
 * @param tokens Token list to validate
 * @return int 1 if valid, 0 if invalid
 */
int	validate_token_list(t_shell *shell, t_token *tokens)
{
	t_token	*current;

	if (!tokens)
		return (1);
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			if (!current->prev || !current->next || \
				current->next->type == TOKEN_PIPE)
			{
				ft_putstr_fd(\
				"minishell: syntax error near unexpected token `|'\n", 2);
				shell->exit_status = 258;
				return (0);
			}
		}
		current = current->next;
	}
	return (1);
}
