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

static t_token	*add_token_to_list(t_token **head, t_token **current,
		const char *input, char *value)
{
	t_token	*new_token;

	new_token = create_token(get_token_type(input), value);
	free(value);
	if (!new_token)
		return (NULL);
	if (!*head)
	{
		*head = new_token;
		*current = *head;
	}
	else
	{
		(*current)->next = new_token;
		*current = new_token;
	}
	return (new_token);
}

static t_token	*process_token(const char **input, t_token **head,
		t_token **current)
{
	size_t	len;
	char	*value;
	t_token	*result;

	len = get_token_length(*input);
	value = ft_substr(*input, 0, len);
	if (!value)
		return (NULL);
	result = add_token_to_list(head, current, *input, value);
	*input += len;
	return (result);
}

t_token	*tokenise(const char *input)
{
	t_token	*head;
	t_token	*current;

	head = NULL;
	current = NULL;
	while (input && *input)
	{
		while (*input && ft_isspace(*input))
			input++;
		if (!*input)
			break ;
		if (!process_token(&input, &head, &current))
		{
			free_tokens(head);
			return (NULL);
		}
	}
	return (head);
}
