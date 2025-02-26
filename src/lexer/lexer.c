/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:11:38 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:13:13 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Process input string and build token list
 *
 * @param input Input string to process
 * @param head Current head of token list
 * @return Updated head of token list, or NULL on failure
 * @note
 * -> input: mkdir test
 * -> output (t_token - linked list): 
 *               head
 *                 |
 *                 v
 * NULL <---- [0x1000] <-----> [0x2000] <-----> [0x3000] -----> NULL
 *               |                |                 |
 *              |                |                  |
 *      value: "mkdir"      value: NULL         value: "test"
 *     type: IDENTIFIER   type: AND             type: IDENTIFIER
 *
 */
static t_token	*process_input_tokens(t_shell *shell, const char *input, \
									t_token *head)
{
	int		err;

	err = 0;
	while (*input)
	{
		if (err)
			return (clean_token_lst(&head), NULL);
		if (ft_isspace(*input))
			ft_skipspace(&input);
		else if (!ft_strncmp(input, "<", 1) || !ft_strncmp(input, ">", 1) || \
				!ft_strncmp(intput, "|", 1) || !ft_strncmp(intput, "&&", 2) || \
				!ft_strncmp(input, "(", 1) || !ft_strncmp(input, ")", 1))
			err = (!handle_separator((char **)&input, &head) && 1);
		else
			err = (!append_identifier(shell, (char **)&input, &head) && 1);
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
t_token	*tokenise(t_shell *shell, const char *input)
{
	t_token	*head;

	if (!input)
		return (NULL);
	if (!init_lexer(shell))
		return (NULL);
	head = NULL;
	head = process_input_tokens(shell, input, head);
	free(shell->line);
	shell->line = NULL;
	return (head);
}
