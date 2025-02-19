/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:09:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 20:57:04 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

/**
 * @brief Adds a new token to the end of token list
 *
 * Maintains the linked list structure of tokens by:
 * - Handling empty list case (head is NULL)
 * - Traversing to end of existing list
 * - Attaching new token
 *
 * @param head      Current head of token list
 * @param new_token Token to add to the list
 * @return Updated head of token list, or NULL on failure
 *
 * @note Safe to call with NULL head (creates new list)
 * @note Returns NULL if new_token is NULL
 * @note Sets new_token->prev to NULL when creating new list
 * @note Sets new_token->next to NULL for last token in list
 */
static t_token	*add_token_to_list(t_token *head, t_token *new_token)
{
	t_token	*current;

	if (!new_token)
		return (NULL);
	if (!head)
	{
		new_token->prev = NULL;
		return (new_token);
	}
	current = head;
	while (current->next)
		current = current->next;
	current->next = new_token;
	new_token->prev = current;
	new_token->next = NULL;
	return (head);
}

/**
 * @brief Creates a single token from input string
 *
 * Process:
 * 1. Extracts token value from input
 * 2. Determines token type
 * 3. Creates token structure
 *
 * @param input Input string to process
 * @param len   Length of token to extract
 * @return New token structure, or NULL on failure
 *
 * @note Handles memory cleanup on failure
 * @note Returns NULL for EOF tokens
 * @see extract_token() for token value extraction
 * @see get_token_type() for token type determination
 */
static t_token	*process_single_token(const char *input, size_t len)
{
	t_token	*new_token;
	char	*value;
	bool	in_single_quotes;

	value = extract_token(input, len, &in_single_quotes);
	if (!value)
		return (NULL);
	if (value[0] == '\0')
	{
		free(value);
		return (NULL);
	}
	new_token = create_token(get_token_type(value), value);
	if (!new_token)
	{
		free(value);
		return (NULL);
	}
	new_token->in_single_quotes = in_single_quotes;
	free(value);
	return (new_token);
}

/**
 * @brief Creates and adds a new token to the token list
 *
 * Handles the token creation process by:
 * 1. Creating a new token from input
 * 2. Adding it to the existing token list
 * 3. Managing memory cleanup on failures
 *
 * @param input Input string to process
 * @param len   Length of token to extract
 * @param head  Current head of token list
 * @return Updated head of token list, or NULL on failure
 *
 * @note Frees existing token list on process_single_token failure
 * @note Frees new token on add_token_to_list failure
 * @see process_single_token() for token creation
 * @see add_token_to_list() for list management
 */
static t_token	*handle_token_creation(const char *input,
			size_t len, t_token *head)
{
	t_token	*new_token;

	new_token = process_single_token(input, len);
	if (!new_token)
	{
		free_tokens(head);
		return (NULL);
	}
	head = add_token_to_list(head, new_token);
	if (!head)
	{
		free_tokens(new_token);
		return (NULL);
	}
	return (head);
}

/**
 * @brief Process input string and build token list
 *
 * @param input Input string to process
 * @param head Current head of token list
 * @return Updated head of token list, or NULL on failure
 */
static t_token	*process_input_tokens(const char *input, t_token *head)
{
	size_t	len;

	while (*input)
	{
		input = skip_whitespace(input);
		if (!*input)
			break ;
		len = get_token_length_with_state(input);
		if (len == 0)
			break ;
		head = handle_token_creation(input, len, head);
		if (!head)
			return (NULL);
		input += len;
	}
	return (head);
}

/**
 * @brief Tokenizes input string into linked list of tokens
 *
 * Main tokenization process:
 * 1. Skips whitespace
 * 2. Determines token length
 * 3. Processes individual tokens via handle_token_creation
 * 4. Builds token list
 *
 * Used by the parser to break command line into processable tokens
 * for AST construction.
 *
 * @param input Command line string to tokenize
 * @return Head of token list, or NULL on failure
 *
 * @note Handles memory cleanup on any failure
 * @note Returns NULL for empty input
 * @see handle_token_creation() for token creation and list management
 * @see process_single_token() for individual token creation
 * @see add_token_to_list() for list building
 *
 * Example token sequence:
 * "ls -l | grep foo" becomes:
 * WORD(ls) -> OPTION(-l) -> PIPE(|) -> WORD(grep) -> WORD(foo)
 */
t_token	*tokenise(const char *input)
{
	t_token	*head;

	if (!input)
		return (NULL);
	head = NULL;
	return (process_input_tokens(input, head));
}
