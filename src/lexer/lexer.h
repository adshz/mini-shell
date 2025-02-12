/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 17:09:24 by szhong            #+#    #+#             */
/*   Updated: 2025/01/27 18:51:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_H
# define LEXER_H
# include <stdlib.h>
# include "libft.h"
# include <stdbool.h>

/* Forward declarations */
struct s_shell;
typedef struct s_shell t_shell;

/* Tokeniser States */
/**
 * @brief Tokeniser state especial for quote and escape handling 
 *
 * Tracks the current parsing state during tokenisation, particularly
 * for handling quoted strings and escaped characters correctly.
 *
 * @param STATE_NORMAL          Default state, processing regular input
 * @param STATE_IN_SINGLE_QUOTE Inside single quotes, everything as literal
 * @param STATE_IN_DOUBLE_QUOTE Inside double quotes, variables can be expanded
 * @param STATE_IN_BACKSLASH    After backslash, escaping next character
*/
typedef enum e_tokeniser_state
{
	STATE_NORMAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE,
	STATE_IN_BACKSLASH
}	t_tokeniser_state;

/* Token types */
/**
 * @brief Token types for lexical analysis
 *
 * Defines all possible token types that can be identified during
 * command line tokenisation. Used by the lexer to classify input components
 *
 * @param TOKEN_WORD         Words, command names, arguments, filenames
 * @param TOKEN_PIPE         Pipe operator (|)
 * @param TOKEN_REDIRECT_IN  Input redirection operator (<)
 * @param TOKEN_REDIRECT_OUT Output redirection operator (>)
 * @param TOKEN_APPEND       Append redirection operator (>>)
 * @param TOKEN_HEREDOC      Here document operator (<<)
 * @param TOKEN_AND          Logical AND operator (&&)
 * @param TOKEN_OR           Logical OR operator (||)
 * @param TOKEN_SEMICOLON    Command separator operator (;)
 * @param TOKEN_NEWLINE      Newline character (\n)
 * @param TOKEN_EOF          End of input marker
*/
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_SEMICOLON,
	TOKEN_NEWLINE,
	TOKEN_EOF
}	t_token_type;

/* Token Structure*/
/**
 * @brief Token structure for lexical analysis
 *
 * Represents a single token in the input stream, storing its value
 * type, and link to the next token in the sequence.
 *
 * @param value	Actual string content of the token (e.g. command name, flag)
 * @param type	Type classification of token (e.g. TOKEN_WORD, TOKEN_PIPE..)
 * @param next	Pointer to next token in the sequence, NULL if last token
 * @param prev	Pointer to previous token in the sequence, NULL if first token
*/
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

/* Function Prototypes */

/* Tokenization */
/**
 * @brief Main tokenization function
 * @param input Command line to tokenize
 * @return Head of token list or NULL on error
 */
t_token				*tokenise(const char *input, t_shell *shell);

/* Token Creation and Management */
/**
 * @brief Creates new token with given type and value
 * @param type Token type to set
 * @param value String value for token
 * @return New token or NULL on error
 */
t_token				*create_token(t_token_type type, const char *value);

/**
 * @brief Frees token list and associated memory
 * @param head Head of token list to free
 */
void				free_tokens(t_token *head);

/* Token Type Handling */
/**
 * @brief Determines if character is a shell special character
 * @param c Character to check
 * @return 1 if special character, 0 otherwise
 */
int					is_special_char(char c);

/**
 * @brief Gets token type from token value
 * @param value String to analyze
 * @return Appropriate token type
 */
t_token_type		get_token_type(const char *value);

/* State Management */
/**
 * @brief Determines next tokenizer state
 * @param current_state Current state
 * @param c Character being processed
 * @return Next tokenizer state
 */
t_tokeniser_state	get_next_state(t_tokeniser_state current_state, char c);

/**
 * @brief Calculates length of next token
 * @param input String to analyze
 * @return Length of next token
 */
size_t				get_token_length_with_state(const char *input);

/**
 * @brief Extracts token from input string
 * @param input Input string
 * @param len Length to extract
 * @return New string containing token or NULL on error
 */
char				*extract_token(const char *input, size_t len, t_shell *shell);

/* State Transitions */
/**
 * @brief Determines next tokenizer state
 * @param current_state Current state
 * @param c Character being processed
 * @return Next tokenizer state
 */
t_tokeniser_state	get_next_state(t_tokeniser_state current_state, char c);

t_tokeniser_state	handle_normal_state(char c);
t_tokeniser_state	handle_double_quote_state(char c, \
												int *was_backslash);
int	is_operator(const char *str);

/**
 * @brief Advances input pointer past whitespace characters
 * @param input String to process
 * @return Pointer to first non-whitespace character
 * @see ft_isspace() for whitespace determination
 * @note Safe to call with NULL input
 * @see process_input_tokens() for token processing in tokeniser.c
 */
const char	*skip_whitespace(const char *input);
#endif
