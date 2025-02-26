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
