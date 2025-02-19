/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_TYPES_H
# define LEXER_TYPES_H

# include <stdbool.h>
# include "libft.h"

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
 * @param value  Actual string content of the token (e.g. command name, flag)
 * @param type   Type classification of token (e.g. TOKEN_WORD, TOKEN_PIPE..)
 * @param next   Pointer to next token in the sequence, NULL if last token
 * @param prev   Pointer to previous token in the sequence, NULL if first token
 * @param in_single_quotes Indicates if the token was in single quotes
*/
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
	bool			in_single_quotes;
}	t_token;
#endif
