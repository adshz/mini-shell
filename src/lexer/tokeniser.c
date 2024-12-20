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

static t_tokenizer_state get_next_state(t_tokenizer_state current_state, char c)
{
	if (current_state == STATE_NORMAL)
	{
		if (c == '\'')
			return STATE_IN_SINGLE_QUOTE;
		if (c == '\"')
			return STATE_IN_DOUBLE_QUOTE;
	}
	else if ((current_state == STATE_IN_SINGLE_QUOTE && c == '\'') ||
			(current_state == STATE_IN_DOUBLE_QUOTE && c == '\"'))
		return STATE_NORMAL;
	return current_state;
}

static size_t get_token_length_with_state(const char *input)
{
	size_t len = 0;
	t_tokenizer_state state = STATE_NORMAL;
	
	while (input[len])
	{
		state = get_next_state(state, input[len]);
		if (state == STATE_NORMAL && ft_isspace(input[len]) && len > 0)
			break;
		if (state == STATE_NORMAL && len == 0 && is_special_char(input[len]))
		{
			if ((input[len] == '|' && input[len + 1] == '|') ||
				(input[len] == '&' && input[len + 1] == '&') ||
				(input[len] == '<' && input[len + 1] == '<') ||
				(input[len] == '>' && input[len + 1] == '>'))
				return 2;
			return 1;
		}
		if (state == STATE_NORMAL && len > 0 && is_special_char(input[len]))
			break;
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

static t_token *create_token_with_type(const char *value, t_token_type type)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return NULL;
	}
	token->type = type;
	token->next = NULL;
	return token;
}

t_token *tokenise(const char *input)
{
	t_token *head = NULL;
	t_token *current = NULL;
	
	while (input && *input)
	{
		// Skip whitespace
		while (*input && ft_isspace(*input))
			input++;
		if (!*input)
			break;
		
		// Get token length considering quotes and special characters
		size_t len = get_token_length_with_state(input);
		if (len == 0)
			break;
		
		// Extract and create token
		char *value = extract_token(input, len);
		if (!value)
		{
			free_tokens(head);
			return NULL;
		}
		
		// Create token with appropriate type
		t_token *new_token = create_token_with_type(value, get_token_type(input));
		free(value);
		
		if (!new_token)
		{
			free_tokens(head);
			return NULL;
		}
		
		// Add token to list
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
