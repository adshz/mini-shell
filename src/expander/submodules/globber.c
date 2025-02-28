/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   globber.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:37:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:49:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

size_t	ft_mutli_arr_strs_count(char ***strs_arrays)
{
	size_t	i;
	size_t	j;
	size_t	strs_count;

	i = 0;
	strs_count = 0;
	while (strs_arrs[i])
	{
		j = 0;
		while (str_arrs[i][j])
			j++;
		strs_count += j;
		i++;
	}
	return (strs_count);
}

char	**ft_flattern_str_arrays(char ***str_arrays)
{
	size_t	i;
	size_t	j;
	size_t	strs_count;
	char	**joined;

	strs_count = ft_mutli_arr_strs_count(str_arrays);
	joined = (char **)ft_calloc(strs_count + 1, sizeof(char *));
	i = 0;
	strs_count = 0;
	while (str_arrays[i])
	{
		j = 0;
		while (str_arrays[i][j])
		{
			joined[strs_count + j] = ft_strdup(str_arrays[i][j]);
			j++;
		}
		strs_count += j;
		i++;
	}
	return (ft_free_3d_array(str_arrays), joined);
}

size_t	ft_pattern_match_count(char *pattern)
{
	DIR				*dir;
	size_t			match_count;
	struct dirent	*entry;

	match_count = 0;
	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (ft_is_wildcard(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

static char	**globber_helper(char *str)
{
	char			**returned;
	DIR				*dir;
	struct dirent	*entry;
	size_t			match_count;

	match_count = ft_pattern_match_count(str);
	if (!ft_contains_asterisk(str) || !match_count)
	{
		returned = (char **)ft_calloc(2, sizeof(char *));
		returned[0] = ft_strdup(str);
		return (returned);
	}
	else
	{
		dir = opendir(".");
		returned = (char **)ft_calloc(match_count + 1, sizeof(char *));
		match_count = 0;
		while (ft_set_direntry(&entry, dir) && entry)
			if (ft_is_wildcard(str, entry->d_name) && \
				ft_should_show_file(str, entry->d_name))
				returned[match_count++] = ft_strdup(entry->d_name);
	}
	return (returned);
}

char	**ft_globber(char **expanded)
{
	size_t	i;
	size_t	expanded_len;
	char	***globbed;

	expanded_len = ft_array_len(expanded);
	globbed = (char ***)ft_calloc(expanded_len + 1, sizeof(char **));
	i = 0;
	while (expanded[i])
	{
		globbed[i] = globber_helper(expanded[i]);
		i++;
	}
	return (ft_free_2d_array(expanded), ft_flattern_str_arrays(globbed));
}
