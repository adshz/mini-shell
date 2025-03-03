/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 19:27:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 19:30:03 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

char	*normal_str_handler(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$' )
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*single_quotes_handler(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	(*i)++;
	while (str[*i] != '\'')
		(*i)++;
	(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*double_quote_str(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] != '"' && str[*i] != '$')
		(*i)++;
	return (ft_substr(str, start, *i - start));
}

char	*double_quotes_handler(t_shell *shell, char *str, size_t *i)
{
	char	*ret;

	ret = ft_strdup("\"");
	(i++);
	while (str[*i] != '"')
	{
		if (str[*i] == '$')
			ret = ft_strjoin_free_both(ret, dollar_handler(shell, str, i));
		else
			ret = ft_strjoin_free_both(ret, double_quote_str(str, i));
	}
	(*i)++;
	return (ft_strjoin_free_both(ret, "\""));
}
