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

	ft_putstr_fd("DEBUG [skip_word]: Starting at position ", 2);
	ft_putnbr_fd(*i, 2);
	ft_putstr_fd(" with char '", 2);
	ft_putchar_fd(str[*i], 2);
	ft_putstr_fd("'\n", 2);

	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '=')
		{
			ft_putstr_fd("DEBUG [skip_word]: Found equals sign at position ", 2);
			ft_putnbr_fd(*i, 2);
			ft_putstr_fd("\n", 2);
			(*i)++;
			break;
		}
		if (str[*i] != '\'' && str[*i] != '"')
			(*i)++;
		else
		{
			quotes = str[(*i)++];
			ft_putstr_fd("DEBUG [skip_word]: Found quote '", 2);
			ft_putchar_fd(quotes, 2);
			ft_putstr_fd("' at position ", 2);
			ft_putnbr_fd(*i - 1, 2);
			ft_putstr_fd("\n", 2);
			while (str[*i] && str[*i] != quotes)
				(*i)++;
			if (str[*i])
				(*i)++;
		}
	}
	ft_putstr_fd("DEBUG [skip_word]: Ended at position ", 2);
	ft_putnbr_fd(*i, 2);
	ft_putstr_fd("\n", 2);
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
			ft_putstr_fd("DEBUG [allocate]: Allocating for word from ", 2);
			ft_putnbr_fd(start, 2);
			ft_putstr_fd(" to ", 2);
			ft_putnbr_fd(i, 2);
			ft_putstr_fd(" (length ", 2);
			ft_putnbr_fd(i - start + 1, 2);
			ft_putstr_fd(")\n", 2);
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

char	**expand_and_split(char *str)
{
	size_t	count;
	char	**array;
	char	**tofree;
	size_t	i;

	if (!str)
		return (NULL);
	i = 0;
	count = 0;
	ft_putstr_fd("DEBUG [split]: Input string: '", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("'\n", 2);
	while (str[i])
	{
		if (str[i] != ' ' && ++count)
			skip_word(str, &i);
		while (str[i] && str[i] == ' ')
			i++;
	}
	ft_putstr_fd("DEBUG [split]: Word count: ", 2);
	ft_putnbr_fd(count, 2);
	ft_putstr_fd("\n", 2);
	array = ft_calloc(count + 1, sizeof(char *));
	tofree = ft_allocate_array(str, array);
	if (!array || !count)
		return (ft_free_2d_array(tofree), NULL);
	return (ft_filler(str, array));
}

void	fill_words(const char *str, char **array, size_t *i, size_t j)
{
	char	quotes;
	size_t	k;

	k = 0;
	ft_putstr_fd("DEBUG [fill_words]: Filling word ", 2);
	ft_putnbr_fd(j, 2);
	ft_putstr_fd(" starting at position ", 2);
	ft_putnbr_fd(*i, 2);
	ft_putstr_fd("\n", 2);

	while (str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '=')
		{
			ft_putstr_fd("DEBUG [fill_words]: Found equals sign, adding to word\n", 2);
			array[j][k++] = str[(*i)++];
			break;
		}
		if (str[*i] != '\'' && str[*i] != '"')
			array[j][k++] = str[(*i)++];
		else
		{
			quotes = str[(*i)++];
			ft_putstr_fd("DEBUG [fill_words]: Processing quote '", 2);
			ft_putchar_fd(quotes, 2);
			ft_putstr_fd("'\n", 2);
			array[j][k++] = quotes;
			while (str[*i] && str[*i] != quotes)
				array[j][k++] = str[(*i)++];
			if (str[*i])
				array[j][k++] = str[(*i)++];
		}
	}
	array[j][k] = '\0';
	ft_putstr_fd("DEBUG [fill_words]: Word ", 2);
	ft_putnbr_fd(j, 2);
	ft_putstr_fd(" is now '", 2);
	ft_putstr_fd(array[j], 2);
	ft_putstr_fd("'\n", 2);
}

char	**ft_filler(const char *str, char **array)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	ft_putstr_fd("DEBUG [filler]: Starting to fill array\n", 2);
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
	ft_putstr_fd("DEBUG [filler]: Filled ", 2);
	ft_putnbr_fd(j, 2);
	ft_putstr_fd(" words\n", 2);
	return (array);
}