/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_classify.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:20:10 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:20:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_CLASSIFY_H
# define LEXER_CLASSIFY_H
# include "lexer_types.h"

t_token_type	determine_separator_type(char *str);
t_token_type	classify_token(char *str);
#endif
