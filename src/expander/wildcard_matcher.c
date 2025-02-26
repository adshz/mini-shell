/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_matcher.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:36:25 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:37:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

bool	is_pattern_match(char **pattern, char **last_wildcard, \
					char **last_match, char **str)
{
	if (**pattern == **str)
	{
		(*pattern)++;
		(*str)++;
		return (true);
	}
	else if (*last_wildcard)
	{
		*str = ++(*last_match);
		*pattern = *last_wildcard;
		return (true);
	}
	else
		return (false);
}

void	pattern_quotes_handler(char **pattern, char *quotes)
{
	if (**pattern == '"' || **pattern == '\'')
	{
		if (!*quotes)
			*quotes = *(*pattern)++;
		else
		{
			if (*quotes == **pattern)
			{
				*quotes = 0;
				(*pattern)++;
			}
		}
	}
}

bool	wildcard_handler(char **pattern, char **last_wildcard, \
					char **last_match, char *str)
{
	while (**pattern == '*')
		(*pattern)++;
	if (!**pattern)
		return (true);
	*last_wildcard = *pattern;
	*last_match = str;
	return (false);
}

bool	ft_is_wildcard(char *pattern, char *str)
{
	char	*last_wildcard;
	char	*last_match;
	char	quotes;

	quotes = 0;
	last_wildcard = NULL;
	last_match = NULL;
	while (*str)
	{
		pattern_quotes_handler(&pattern, &quotes);
		if (*pattern == '*' && !quotes && \
			wildcard_handler(&pattern, &last_wildcard, &last_match, str))
			return (true);
		else if (!is_pattern_match(&pattern, &last_wilcard, &last_match, &str))
			return (false);
	}
	if (*pattern == '*')
		while (*pattern == '*')
			pattern++;
	return (!*pattern);
}
