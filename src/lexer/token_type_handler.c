/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_type_handler.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:59:20 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 18:07:17 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer.h"
#include "shell.h"
/**
 * @brief Checks if character is a shell special character
 *
 * Special characters are:
 * - | (pipe)
 * - < (input redirect)
 * - > (output redirect)
 * - & (background)
 *
 * @param c Character to check
 * @return 1 if character is special, 0 otherwise
 *
 * @note Used during tokenization to identify operator boundaries
 */
int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

/**
 * @brief Checks if string is a shell operator
 *
 * Recognizes operators:
 * - || (OR)
 * - && (AND)
 * - << (heredoc)
 * - >> (append redirect)
 *
 * @param str String to check
 * @return 1 if string is an operator, 0 otherwise
 *
 * @note Safe to call with NULL or empty string
 * @note Only checks for two-character operators
 */
int	is_operator(const char *str)
{
	if (!str || !*str)
		return (0);
	if (ft_strncmp(str, "||", 2) == 0)
		return (1);
	if (ft_strncmp(str, "&&", 2) == 0)
		return (1);
	if (ft_strncmp(str, "<<", 2) == 0)
		return (1);
	if (ft_strncmp(str, ">>", 2) == 0)
		return (1);
	return (0);
}

/**
 * @brief Determines token type from token value
 *
 * Maps token strings to their corresponding types:
 * - | -> TOKEN_PIPE
 * - < -> TOKEN_REDIRECT_IN
 * - > -> TOKEN_REDIRECT_OUT
 * - >> -> TOKEN_REDIRECT_APPEND
 * - << -> TOKEN_HEREDOC
 * - && -> TOKEN_AND
 * - || -> TOKEN_OR
 * - others -> TOKEN_WORD
 *
 * @param value String value to analyze
 * @return Appropriate token type, TOKEN_EOF for NULL/empty string
 *
 * @note Used during token creation in tokenizer
 * @see t_token_type for all possible token types
 * @see create_token() where this function is used
 */
t_token_type	get_token_type(const char *value)
{
	if (!value || !*value)
		return (TOKEN_EOF);
	if (ft_strcmp(value, "|") == 0)
		return (TOKEN_PIPE);
	if (ft_strcmp(value, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	if (ft_strcmp(value, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	if (ft_strcmp(value, ">>") == 0)
		return (TOKEN_APPEND);
	if (ft_strcmp(value, "<<") == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(value, "&&", 2) == 0)
		return (TOKEN_AND);
	if (ft_strncmp(value, "||", 2) == 0)
		return (TOKEN_OR);
	return (TOKEN_WORD);
}
