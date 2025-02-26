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
# include "includes/lexer_init.h"
# include "includes/lexer_scan.h"
# include "includes/lexer_classify.h"
# include "includes/lexer_build.h"
# include "includes/lexer_validate.h"
# include "includes/lexer_cleanup.h"

/* Main Tokenization Function */
/**
 * @brief Main tokenization function
 * @param input Command line to tokenize
 * @return Head of token list or NULL on error
 */
t_token	*tokenise(const char *input);

#endif
