/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_scan.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 01:38:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 01:49:21 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Check if a character is a separator
 *
 * @param s Pointer to the character to check
 * @return int 1 if separator, 0 otherwise
 */
int	is_separator(char *s)
{
	if (!s)
		return (0);
	if (!ft_strncmp(s, "&&", 2) || *s == ' ' || *s == '\t' || \
		*s == '<' || *s == '>' || *s == '|' || *s == '(' || *s == ')')
		return (1);
	return (0);
}

/**
 * @brief Check if a character is a quote
 *
 * @param c Character to check
 * @return int 1 if quote, 0 otherwise
 */
int	is_single_or_double_quote(char c)
{
	if (c == '\'' || c == '"')
		return (1);
	return (0);
}

/**
 * @brief Skip over quoted text in the input
 *
 * @param line Input string
 * @param i Current position (will be updated)
 * @return bool true if quotes are properly closed, false otherwise
 */
bool	lexer_skip_quotes(char *line, size_t *i)
{
	char	quote;

	quote = line[*i];
	if (ft_strchr(line + *i + 1, quote))
	{
		(*i)++;
		while (line[*i] != quote)
			(*i)++;
		(*i)++;
		return (true);
	}
	return (false);
}

/**
 * @brief Find the length of the current token
 *
 * @param line Input string
 * @param shell Shell structure for error reporting
 * @return size_t Length of the token, 0 on error
 */
size_t	get_token_length(char *line, t_shell *shell)
{
	size_t	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] && !is_separator(&line[i]))
	{
		if (is_single_or_double_quote(line[i]))
		{
			if (!lexer_skip_quotes(line, &i))
			{
				error_quote_printer(shell, line[i]);
				return (0);
			}
		}
		else
			i++;
	}
	return (i);
}
