/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_types.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:08:38 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_TYPES_H
# define LEXER_TYPES_H

# include "../lexer.h"

/**
 * @brief Token types for lexical analysis
 *
 * Defines all possible token types that can be identified during
 * command line tokenisation. Used by the lexer to classify input components
 *
 * @param TOKEN_PIPE		Pipe operator ('|')
 * @param TOKEN_AND			Logical AND operator ('&&')
 * @param TOKEN_OR			Logical OR operator ('||')
 * @param TOKEN_LESS		Lesser Operator ('<')
 * @param TOKEN_DLESS		Double Lesser Operator ('<<') Heredoc Mode Token
 * @param TOKEN_GREAT		Greater Operator ('>') Redirection Output Token
 * @param TOKEN_DGREAT		Double Greater Operator ('>>') Append Output Token
 * @param TOKEN_O_PARENT	Open Parenthesis ('(')
 * @param TOKEN_C_PARENT	Close Parentthesis (')')
 * @param TOKEN_IDENTIFIER	WORD. eg 'mkdir test && cd test' mkdir->value, type
 * 							is TOKEN_IDENTIFIER
 * @param TOKEN_EOF          End of input marker
*/
typedef enum e_token_type
{
	TOKEN_IDENTIFIER,
	TOKEN_LESS,
	TOKEN_GREAT,
	TOKEN_DLESS,
	TOKEN_DGREAT,
	TOKEN_O_PARENT,
	TOKEN_C_PARENT,
	TOKEN_PIPE,
	TOKEN_AND,
	TOKEN_OR,
	TOKEN_EOF,
	TOKEN_UNKNOWN,
	TOKEN_NEWLINE
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
*/
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;
#endif
