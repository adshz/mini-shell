/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <stdlib.h>
#include "shell.h"
#include "lexer.h"
#include "libft.h"

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = (t_token *)malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->next = NULL;
	return (token);
}

t_token_type	get_token_type(const char *str)
{
	if (!str)
		return (TOKEN_EOF);
	if (str[0] == '|' && str[1] == '|')
		return (TOKEN_OR);
	if (str[0] == '&' && str[1] == '&')
		return (TOKEN_AND);
	if (str[0] == '|')
		return (TOKEN_PIPE);
	if (str[0] == '<' && str[1] == '<')
		return (TOKEN_HEREDOC);
	if (str[0] == '>' && str[1] == '>')
		return (TOKEN_APPEND);
	if (str[0] == '<')
		return (TOKEN_REDIRECT_IN);
	if (str[0] == '>')
		return (TOKEN_REDIRECT_OUT);
	if (str[0] == ';')
		return (TOKEN_SEMICOLON);
	if (str[0] == '\n')
		return (TOKEN_NEWLINE);
	return (TOKEN_WORD);
}

size_t	get_token_length(const char *input)
{
	size_t	len;

	len = 0;
	if (is_special_char(input[0]))
	{
		if ((input[0] == '|' && input[1] == '|')
			|| (input[0] == '&' && input[1] == '&')
			|| (input[0] == '<' && input[1] == '<')
			|| (input[0] == '>' && input[1] == '>'))
			return (2);
		return (1);
	}
	while (input[len] && !ft_isspace(input[len])
		&& !is_special_char(input[len]))
		len++;
	return (len);
}

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free(tokens->value);
		free(tokens);
		tokens = next;
	}
} 
