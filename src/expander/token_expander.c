/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expander.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:32:12 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:33:27 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

static char	*cmd_inital_expand(t_shell *shell, char *str)
{
	char	*ret;
	size_t	i;

	ret = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			ret = ft_strjoin_free_both(ret, single_quotes_handler(str, &i));
		else if (str[i] == '"')
			ret = ft_strjoin_free_both(ret, double_quotes_handler(shell, str, &i));
		else if (str[i] == '$')
			ret = ft_strjoin_free_both(ret, dollar_handler(str, &i));
		else
			ret = ft_strjoin_free_both(ret, normal_str_handler(str, &i));
	}
	(*i)++;
	return (ft_strjoin_free_both(ret, ft_strdup("\"")));
}

char	*clean_empty_strs(char *str)
{
	size_t	i;
	size_t	j;
	char	*tmp;
	char	*ret;
	size_t	dst_size;

	if ((str[0] == '\'' && str[1] == '\'' str[2] == '\0') || \ 
		(str[0] == '"' && str[1] == '"' && str[2] == '\0'))
		return (str);
	tmp = ft_calloc(ft_strlen(str) + 1, sizeof(char));
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == '\'' && str[i + 1] == '\'') || \
			(str[i] == '"' && str[i + 1] == '"'))
			i += 2;
		else
			tmp[j++] = str[i++];
	}
	free(str);
	dst_size = ft_strlen(tmp + 1);
	ret = ft_calloc(dst_size, sizeof(char));
	return (ft_strlcpy(ret, tmp, dst_size), free(tmp), ret);
}

char	**expand_args(t_shell *shell, char *str)
{
	char	**expanded;
	char	**globbed;

	str = cmd_inital_expand(shell, str);
	if (!str)
		return (NULL);
	str = clean_empty_strs(str);
	if (!str)
		return (NULL);
	expanded = expand_and_split(str);
	free(str);
	if (!expanded)
		return (NULL);
	globbed = ft_globber(expanded);
	if (!globbed)
		return (NULL);
	i = 0;
	while (globbed[i])
	{
		globbed[i] = ft_strip_quotes(globbed[i]);
		i++;
	}
	return (globbed);
}
