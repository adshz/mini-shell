/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 01:30:44 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 01:36:54 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Initialize the lexer state
 * 
 * Sets up any necessary state for beginning the lexing process.
 * This could include setting up state variables, initializing token list, etc.
 *
 * @param shell Pointer to shell structure
 * @return int 1 for success, 0 for failure
 */
int	init_lexer(t_shell *shell)
{
	if (!shell)
		return (0);
	if (shell->tokens)
		ft_clean_token(&(shell->tokens));
	shell->tokens = NULL;
	return (1);
}

t_token	*ft_create_new_token(char *value, t_token_type type)
{
	t_token	*new_token;

	new_token = (t_token *)ft_calloc(1, sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = value;
	new_token->type = type;
	return (new_token);
}
