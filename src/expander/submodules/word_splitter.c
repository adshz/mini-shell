/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   word_splitter.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:34:10 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:35:58 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

static void	skip_word(const char *str, size_t *i)
{
	char	quotes;

	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] != '\'' && str[*i] != '"')
			(*i)++;
		else
		{
			quotes = str[(*i)++];
			while (str[(*i)] != quotes)
				(*i)++;
			(*i)++;
		}
	}
}

char	**expand_and_split(char *str)
{
	size_t	count;
	char	**array;
	char	**tofree;
	size_t	i;

	if (!s)
		return (NULL);
	i = 0;
	count = 0;
	while (str[i])
	{
		if (s[i] != ' ' && ++count)
			skip_word(str, &i);
		while (str[i] && str[i] == ' ')
			i++;
	}
	array = ft_calloc(count + 1, sizeof(char));
	tofree = ft_allcoate_array(str, array);
	if (!array || !count)
		return (ft_free_2d_array(to_free), NULL);
	return (ft_filler(str, array));
}

static char	**ft_allocate_array(char const *str, char **array)
{
	size_t	start;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != ' ')
		{
			start = i;
			skip_word(str, &i);
			array[j] = ft_calloc(i - start + 1, sizeof(char));
			if (!array[j])
				return (NULL);
			j++;
		}
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (array);
}

void	fill_words(const char *str, char **array, size_t *i, size_t j)
{
	char	quotes;
	size_t	k;

	k = 0;
	while (str[(*i)] && str[(*i) != ' '])
	{
		if (str[(*i)] != '\'' && str[(*i)] != '"')
			array[j][k++] = str[(*i)++];
		else
		{
			qutoes = str[(*i)++];
			array[j][k++] = quotes;
			while (str[(*i)] != quotes)
				array[j][k++] = str[(*i)++];
			array[j][k++] = s[(*i)++];
		}
	}
}

char	**ft_filler(const char *str, char **array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (str[i] && str[j])
	{
		if (str[i] != ' ')
		{
			fill_words(str, array, &i, j);
			j++;
		}
		while (str[i] && str[i] == ' ')
			i++;
	}
	return (array);
}
