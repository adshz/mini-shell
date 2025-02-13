/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_filename_expander.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:22:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 17:01:01 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"

char	*expand_redirection_filename_variable(t_shell *shell,
		const char *filename)
{
	if (filename[0] != '$')
		return (NULL);
	return (expand_variable(shell, filename + 1));
}

char	*remove_quotes(const char *filename)
{
	size_t	len;

	len = ft_strlen(filename);
	if ((filename[0] != '"' && filename[0] != '\'') || \
		filename[len - 1] != filename[0])
		return (NULL);
	return (ft_substr(filename, 1, len - 2));
}

char	*handle_quoted_variable(t_shell *shell, char *unquoted)
{
	char	*expanded;

	if (unquoted[0] != '$')
		return (unquoted);
	expanded = expand_variable(shell, unquoted + 1);
	free(unquoted);
	return (expanded);
}

char	*handle_quoted_filename(t_shell *shell, const char *filename)
{
	char	*unquoted;
	char	*result;

	unquoted = remove_quotes(filename);
	if (!unquoted)
		return (NULL);
	result = handle_quoted_variable(shell, unquoted);
	return (result);
}
