/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_scan.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:21:02 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:21:36 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_SCAN_H
# define LEXER_SCAN_H
# include "types.h"

int		is_separator(char *s);
int		is_single_or_double_quote(char c);
bool	lexer_skip_quotes(char *line, size_t *i);
size_t	get_token_length(char *line, t_shell *shell);
#endif
