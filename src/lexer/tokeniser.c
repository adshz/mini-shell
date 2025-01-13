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
#include "lexer.h"
#include "shell.h"
#include "libft.h"

static t_tokenizer_state get_next_state(t_tokenizer_state current_state, char c)
{
	static int was_backslash;

	was_backslash = 0;
	if (current_state == STATE_IN_BACKSLASH)
	{
		was_backslash = 0;
		return (STATE_NORMAL);
	}
	if (current_state == STATE_NORMAL)
	{
		if (c == '\\')
		{
			was_backslash = 1;
			return (STATE_IN_BACKSLASH);
		}
		if (c == '\'')
			return (STATE_IN_SINGLE_QUOTE);
		if (c == '\"')
			return (STATE_IN_DOUBLE_QUOTE);
	}
	else if (current_state == STATE_IN_DOUBLE_QUOTE)
	{
		if (c == '\\' && !was_backslash)
		{
			was_backslash = 1;
			return (STATE_IN_BACKSLASH);
		}
		if (c == '\"' && !was_backslash)
			return (STATE_NORMAL);
		was_backslash = 0;
	}
	else if (current_state == STATE_IN_SINGLE_QUOTE && c == '\'')
		return (STATE_NORMAL);
	was_backslash = 0;
	return (current_state);
}

static size_t	get_token_length_with_state(const char *input)
{
	size_t			len;
	t_tokenizer_state	state;

	len = 0;
	state = STATE_NORMAL;
	while (input[len])
	{
		if (state == STATE_NORMAL && input[len] == '\\')
		{
			if (input[len + 1])
			{
				len += 2;
				continue;
			}
		}
		state = get_next_state(state, input[len]);
		if (state == STATE_NORMAL)
		{
			if (len > 0 && (input[len] == '"' || input[len] == '\''))
			{
				len++;
				if (input[len] && !ft_isspace(input[len]) && !is_special_char(input[len]))
					break;
				continue;
			}
			if (ft_isspace(input[len]) && len > 0)
				break;
			if (len == 0 && is_special_char(input[len]))
			{
				if ((input[len] == '|' && input[len + 1] == '|') ||
					(input[len] == '&' && input[len + 1] == '&') ||
					(input[len] == '<' && input[len + 1] == '<') ||
					(input[len] == '>' && input[len + 1] == '>'))
					return (2);
				return (1);
			}
			if (len > 0 && is_special_char(input[len]))
				break;
		}
		len++;
	}
	return (len);
}

static char *extract_token(const char *input, size_t len)
{
	char	*result;
	size_t	i;
	size_t	j;
	t_tokenizer_state state;

	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	state = STATE_NORMAL;

	while (i < len)
	{
		if (state == STATE_IN_DOUBLE_QUOTE)
		{
			if (input[i] == '\\')
			{
				if (i + 1 < len && input[i + 1] == '"')
				{
					i++;
					result[j++] = input[i];
				}
				else
					result[j++] = input[i];
				i++;
				continue;
			}
		}
		else if (input[i] == '\\' && state == STATE_NORMAL)
		{
			i++;
			if (i < len)
				result[j++] = input[i++];
			continue;
		}

		if (input[i] == '"' && state == STATE_NORMAL)
		{
			state = STATE_IN_DOUBLE_QUOTE;
			result[j++] = input[i];  // Keep the quote
		}
		else if (input[i] == '"' && state == STATE_IN_DOUBLE_QUOTE)
		{
			state = STATE_NORMAL;
			result[j++] = input[i];  // Keep the quote
		}
		else if (input[i] == '\'' && state == STATE_NORMAL)
		{
			state = STATE_IN_SINGLE_QUOTE;
			result[j++] = input[i];  // Keep the quote
		}
		else if (input[i] == '\'' && state == STATE_IN_SINGLE_QUOTE)
		{
			state = STATE_NORMAL;
			result[j++] = input[i];  // Keep the quote
		}
		else
			result[j++] = input[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

t_token *tokenise(const char *input)
{
	t_token	*head;
	t_token	*current;
	size_t	len;
	char	*value;
	t_token_type type;

	ft_putstr_fd("\n[DEBUG] Tokenizing input: '", STDERR_FILENO);
	ft_putstr_fd((char *)input, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);

	head = NULL;
	current = NULL;
	while (*input)
	{
		while (ft_isspace(*input))
			input++;
		if (!*input)
			break;

			ft_putstr_fd("[DEBUG] Processing segment: '", STDERR_FILENO);
			ft_putstr_fd((char *)input, STDERR_FILENO);
			ft_putendl_fd("'", STDERR_FILENO);

		len = get_token_length_with_state(input);
		if (len == 0)
			break;

		ft_putstr_fd("[DEBUG] Token length: ", STDERR_FILENO);
		ft_putnbr_fd(len, STDERR_FILENO);
		ft_putchar_fd('\n', STDERR_FILENO);

		value = extract_token(input, len);
		if (!value)
		{
			free_tokens(head);
			return (NULL);
		}

		ft_putstr_fd("[DEBUG] Extracted token: '", STDERR_FILENO);
		ft_putstr_fd(value, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);

		type = get_token_type(value);
		if (type == TOKEN_EOF)  // Stop on syntax error
		{
			free(value);
			free_tokens(head);
			return (NULL);
		}

		t_token *new_token = create_token(type, value);
		free(value);
		if (!new_token)
		{
			free_tokens(head);
			return (NULL);
		}

		ft_putstr_fd("[DEBUG] Created token with value: '", STDERR_FILENO);
		ft_putstr_fd(new_token->value, STDERR_FILENO);
		ft_putendl_fd("'", STDERR_FILENO);

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

	ft_putendl_fd("[DEBUG] Tokenization complete", STDERR_FILENO);
	return (head);
}
