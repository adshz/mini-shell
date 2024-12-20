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

static int is_in_quotes(const char *input, size_t pos)
{
	size_t  i;
	int     quote_count;

	i = 0;
	quote_count = 0;
	while (i < pos)
	{
		if (input[i] == '\'')
			quote_count++;
		i++;
	}
	return (quote_count % 2);
}

static size_t get_word_length(const char *input)
{
	size_t  len;

	len = 0;
	while (input[len])
	{
		if (!is_in_quotes(input, len) && 
			(ft_isspace(input[len]) || is_special_char(input[len])))
			break;
		len++;
	}
	return (len);
}

size_t get_token_length(const char *input)
{
	if (is_special_char(input[0]))
	{
		if ((input[0] == '|' && input[1] == '|')
			|| (input[0] == '&' && input[1] == '&')
			|| (input[0] == '<' && input[1] == '<')
			|| (input[0] == '>' && input[1] == '>'))
			return (2);
		return (1);
	}
	return (get_word_length(input));
}

static char *process_quoted_string(const char *input, size_t len)
{
	(void)len;  // Silence unused parameter warning
	return ft_strdup(input);  // Preserve the original string with quotes
}

t_token *create_token(t_token_type type, const char *value)
{
	t_token *token;
	char    *processed_value;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	
	if (ft_strchr(value, '\'') || ft_strchr(value, '\"'))
		processed_value = process_quoted_string(value, ft_strlen(value));
	else
		processed_value = ft_strdup(value);
	
	if (!processed_value)
	{
		free(token);
		return (NULL);
	}
	token->value = processed_value;
	token->type = type;
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
