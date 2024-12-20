/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
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

static void skip_spaces(const char **input)
{
	while (**input && ft_isspace(**input))
		(*input)++;
}

static size_t get_token_length_with_state(const char *input)
{
	size_t len = 0;
	t_tokenizer_state state = STATE_NORMAL;
	
	while (input[len])
	{
		if (state == STATE_NORMAL)
		{
			if (input[len] == '\'')
				state = STATE_IN_SINGLE_QUOTE;
			else if (input[len] == '\"')
				state = STATE_IN_DOUBLE_QUOTE;
			else if (ft_isspace(input[len]) && !is_special_char(input[len]))
				break;
			else if (is_special_char(input[len]))
			{
				if (len == 0)
				{
					if ((input[len] == '|' && input[len + 1] == '|') ||
						(input[len] == '&' && input[len + 1] == '&') ||
						(input[len] == '<' && input[len + 1] == '<') ||
						(input[len] == '>' && input[len + 1] == '>'))
						return (2);
					return (1);
				}
				break;
			}
		}
		else if (state == STATE_IN_SINGLE_QUOTE)
		{
			if (input[len] == '\'')
				state = STATE_NORMAL;
		}
		else if (state == STATE_IN_DOUBLE_QUOTE)
		{
			if (input[len] == '\"')
				state = STATE_NORMAL;
		}
		len++;
	}
	return len;
}

static char *extract_token(const char *input, size_t len)
{
	char *token = malloc(len + 1);
	if (!token)
		return NULL;
	
	ft_strlcpy(token, input, len + 1);
	return token;
}

t_token *tokenise(const char *input)
{
	t_token *head = NULL;
	t_token *current = NULL;
	
	while (input && *input)
	{
		skip_spaces(&input);
		if (!*input)
			break;
		
		size_t len = get_token_length_with_state(input);
		if (len == 0)
			break;
		
		char *value = extract_token(input, len);
		if (!value)
		{
			free_tokens(head);
			return NULL;
		}
		
		t_token *new_token = create_token(get_token_type(input), value);
		free(value);
		
		if (!new_token)
		{
			free_tokens(head);
			return NULL;
		}
		
		if (!head)
		{
			head = new_token;
			current = head;
		}
		else
		{
			current->next = new_token;
			current = new_token;
		}
		
		input += len;
	}
	
	return head;
}
