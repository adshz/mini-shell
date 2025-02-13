/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 18:07:11 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 18:16:05 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expander.h"

int	is_quoted(const char *str)
{
	return (str && (str[0] == '\'' || str[0] == '"'));
}

static void	handle_string_copy(char *result, const char *str,
		size_t *i, size_t *j, char quote_char)
{
	if (quote_char)
	{
		(*i)++;
		while (str[*i] && str[*i] != quote_char)
		{
			result[*j] = str[*i];
			(*j)++;
			(*i)++;
		}
		if (str[*i])
			(*i)++;
	}
	else
	{
		result[*j] = str[*i];
		(*j)++;
		(*i)++;
	}
}

static char	*process_string(const char *str)
{
	char	*result;
	size_t	i;
	size_t	j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			handle_string_copy(result, str, &i, &j, str[i]);
		else
			handle_string_copy(result, str, &i, &j, 0);
	}
	result[j] = '\0';
	return (result);
}

char	*strip_quotes(char *str)
{
	char	*result;

	if (!str)
		return (NULL);
	result = process_string(str);
	free(str);
	return (result);
}
