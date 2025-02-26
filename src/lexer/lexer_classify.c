/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_classify.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 01:50:01 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:06:13 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Determine the type of a separator token
 *
 * @param str Pointer to the separator character(s)
 * @return t_token_type Type of the separator token
 */
t_token_type	determine_separator_type(char *str)
{
	if (!str || !*str)
		return (TOKEN_EOF);
	if (!ft_strncmp(str, "<<", 2))
		return (TOKEN_DLESS);
	else if (!ft_strncmp(str, ">>", 2))
		return (TOKEN_DGREAT);
	else if (!ft_strncmp(str, "<", 1))
		return (TOKEN_LESS);
	else if (!ft_strncmp(str, ">", 1))
		return (TOKEN_GREAT);
	else if (!ft_strncmp(str, "(", 1))
		return (TOKEN_O_PARENT);
	else if (!ft_strncmp(str, ")", 1))
		return (TOKEN_C_PARENT);
	else if (!ft_strncmp(str, "&&", 2))
		return (TOKEN_AND);
	else if (!ft_strncmp(str, "||", 2))
		return (TOKEN_OR);
	else if (!ft_strncmp(str, "|", 1))
		return (TOKEN_PIPE);
	return (TOKEN_UNKNOWN);
}

/**
 * @brief Determine token type based on content
 *
 * @param str Token content:
 * @return t_token_type Token type
 */
t_token_type	classify_token(char *str)
{
	if (!str)
		return (TOKEN_UNKNOWN);
	if (is_separator(str))
		return (determine_separator_type(str));
	return (TOKEN_IDENTIFIER);
}
