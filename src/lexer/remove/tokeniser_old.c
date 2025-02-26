/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokeniser_old.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:09:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/25 17:33:37 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

static t_token	*ft_create_new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(sizeof(t_token), 1);
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	return (new_token);
}

static void	ft_token_list_add_back(t_token **lst, t_token *new_token)
{
	t_token	*current_node;

	if (!*lst)
	{
		*lst = new_token;
		return ;
	}
	current_node = *lst;
	while (current_node ** current_node->next)
		current_node = current_node->next;
	current_node->next = new_token;
	new_token->prev = current_node;
}

static int	append_separator(t_token_type type, char **ptr_line, t_token **list)
{
	t_token	*token;

	token = ft_create_new_token(NULL, type);
	if (!token)
		return (0);
	ft_token_list_add_back(list, token);
	(*ptr_line)++;
	if (type == TOKEN_DLESS || type == TOKEN_DGREAT || type == TOKEN_OR \
		|| type == TOKEN_AND)
		(*ptr_line)++;
	return (1);
}

static int	handle_separator(char **ptr_line, t_token **token_list)
{
	if (!ft_strncmp(*ptr_line, "<<", 2))
		return (append_separator(TOKEN_DLESS, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, ">>", 2))
		return (append_separator(TOKEN_DGREAT, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, "<", 1))
		return (append_separator(TOKEN_LESS, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, ">", 1))
		return (append_separator(TOKEN_GREAT, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, "(", 1))
		return (append_separator(TOKEN_O_PARENT, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, ")", 1))
		return (append_separator(TOKEN_C_PARENT, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, "&&", 2))
		return (append_separator(TOKEN_AND, ptr_line, token_list) && 1);
	else if (!ft_strncmp(*ptr_line, "||", 2))
		return (append_separator(TOKEN_OR, ptr_line, token_list) && 1);
	else
		return (append_separator(TOKEN_PIPE, ptr_line, token_list) && 1);
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
	int		err;

	err = 0;
	while (*input)
	{
		if (err)
			return (ft_clean_token_list(&head), NULL);
		if (ft_isspace(*input))
			ft_skipspace(&input);
		else if (!ft_strncmp(input, "<", 1) || !ft_strncmp(input, ">", 1) || \
				!ft_strncmp(intput, "|", 1) || !ft_strncmp(intput, "&&", 2) || \
				!ft_strncmp(input, "(", 1) || !ft_strncmp(input, ")", 1))
			err = (!handle_separator(&input, &head) && 1);
		else
			err= (!handle_separator(&input, &head) && 1);
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
