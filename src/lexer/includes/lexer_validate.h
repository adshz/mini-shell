/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_validate.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:18:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:18:40 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_VALIDATE_H
# define LEXER_VALIDATE_H

void	error_quote_printer(t_shell *shell, char c);
int		validate_token_list(t_shell *shell, t_token *tokens);
#endif
