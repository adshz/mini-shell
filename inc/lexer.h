/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:14 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:50:14 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include <stdlib.h>

typedef enum e_token_type
{
    TOKEN_WORD,           // Words and command names
    TOKEN_PIPE,           // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_APPEND,         // >>
    TOKEN_HEREDOC,        // <<
    TOKEN_AND,           // &&
    TOKEN_OR,            // ||
    TOKEN_SEMICOLON,     // ;
    TOKEN_NEWLINE,       // \n
    TOKEN_EOF            // End of input
}   t_token_type;

typedef enum e_tokenizer_state {
	STATE_NORMAL,
	STATE_IN_SINGLE_QUOTE,
	STATE_IN_DOUBLE_QUOTE,
	STATE_IN_BACKSLASH
} t_tokenizer_state;

typedef struct s_token
{
    char            *value;
    t_token_type    type;
    struct s_token  *next;
}   t_token;

// Function prototypes
t_token     	*tokenise(const char *input);
void        	free_tokens(t_token *tokens);
t_token 		*create_token(t_token_type type, const char *value);
t_token_type	get_token_type(const char *str);
size_t			get_token_length(const char *input);
int				is_special_char(char c);
#endif
