/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_text_processor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:32:52 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:34:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
 * Command Text Processing
 *
 * This file processes command text from tokens
 *
 */
#include "parser/parser.h"

// Build raw command string from consecutive identifier tokens
bool	ft_build_raw_command_string(t_shell *shell, char **raw_cmd_ptr)
{
	char	*to_free;

	if (shell->parse_err.type)
		return (false);
	if (!*raw_cmd_ptr)
		*raw_cmd_ptr = ft_strdup("");
	if (!*raw_cmd_ptr)
		return (false);
	while (shell->curr_token && shell->curr_token->type == TOKEN_IDENTIFIER)
	{
		to_free = *raw_cmd_ptr;
		*raw_cmd_ptr = ft_strjoin3(*raw_cmd_ptr, shell->curr_token->value, ' ');
		if (!*raw_cmd_ptr)
			return (free(to_free), false);
		free(to_free);
		ft_get_next_token(shell);
	}
	return (true);
}

// Join strings with a separatori // this could go to common utils
char	*ft_strjoin3(const char *s1, const char *s2, char c)
{
	char	*joined;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	if (!c || !ft_strlen(s1) || !ft_strlen(s2))
		return (ft_strjoin(s1, s2));
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	joined = ft_calloc(total_len, sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i++] = c;
	j = 0;
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	return (joined);
}
