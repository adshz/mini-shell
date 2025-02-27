/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 17:24:23 by szhong            #+#    #+#             */
/*   Updated: 2025/02/27 17:24:26 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

void	ft_skipspace(char **line)
{
	while (**line && ft_isspace(**line))
		(*line)++;
}

void	ft_clean_token_list(t_token **lst)
{
	t_token	*current_node;
	t_token	*next;

	current_node = *lst;
	if (!current_node)
		return ;
	while (current_node)
	{
		free(current_node);
		next = current_node->next;
		free(current_node);
		current_node = next;
	}
	*lst = NULL;
}

/**
 * @brief Creates a new token with specified type and value
 *
 * Allocates and initializes a token structure:
 * 1. Allocates token structure
 * 2. Sets token type
 * 3. Creates copy of value string
 * 4. Initializes next pointer to NULL
 *
 * @param type  Type of token to create (e.g., WORD, PIPE, REDIRECT)
 * @param value String value for the token
 * @return      Pointer to new token, or NULL on allocation failure
 *
 * @note Handles memory cleanup on partial allocation failure
 * @note Creates deep copy of value string
 * @see t_token_type for possible token types
 */
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
	token->prev = NULL;
	token->in_single_quotes = false;
	return (token);
}

/**
 * @brief Frees entire token list and associated memory
 *
 * Cleanup process:
 * 1. Traverses token list
 * 2. Frees token value string
 * 3. Frees token structure
 * 4. Moves to next token
 *
 * @param head Head of token list to free
 *
 * @note Safe to call with NULL head
 * @note Used during error cleanup and final program cleanup
 * @see tokenise() for token list creation
 */
void	free_tokens(t_token *head)
{
	t_token	*current;
	t_token	*next;

	current = head;
	while (current)
	{
		next = current->next;
		free(current->value);
		free(current);
		current = next;
	}
}

/**
 * @brief Advances input pointer past whitespace characters
 *
 * Used to prepare input for token extraction by skipping:
 * - Spaces
 * - Tabs
 * - Newlines
 * - Other whitespace characters
 *
 * @param input String to process
 * @return Pointer to first non-whitespace character
 *
 * @note Safe to call with NULL input
 * @see ft_isspace() for whitespace determination
 */
const char	*skip_whitespace(const char **input)
{
	while (input && ft_isspace(*input))
		input++;
	return (input);
}
