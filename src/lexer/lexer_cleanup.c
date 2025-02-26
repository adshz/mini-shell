/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cleanup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:10:14 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:10:49 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Free a single token
 *
 * @param token Token to free
 */
void	ft_free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

/**
 * @brief Free the entire token list
 *
 * @param list Pointer to the token list head
 */
void	clean_token_lst(t_token **lst)
{
	t_token	*curr_token;
	t_token	*next;

	curr_token = *lst;
	if (!curr_token)
		return ;
	while (curr_token)
	{
		free(curr_token->value);
		next = curr_token->next;
		free(curr_token);
		curr_token = next;
	}
	*lst = NULL;
}
