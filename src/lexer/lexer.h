/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_H
# define LEXER_H

# include "includes/lexer_types.h"
# include "includes/token_handler.h"
# include "includes/state_handler.h"

typedef struct s_token_extract
{
	char				*result;
	size_t				i;
	size_t				j;
	t_tokeniser_state	state;
	t_tokeniser_state	prev_state;
}	t_token_extract;

/* Main Tokenization Function */
/**
 * @brief Main tokenization function
 * @param input Command line to tokenize
 * @return Head of token list or NULL on error
 */
t_token	*tokenise(const char *input);

#endif
