/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:06:36 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:08:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "lexer/lexer.h"

/**
 * @brief Add a token to the end of the token list
 *
 * @param lst Pointer to the token list head
 * @param new_token Token to add
 */
void	ft_token_list_add_back(t_token **lst, t_token *new_token)
{
	t_token	*current_node;

	if (!lst || !new_token)
		return ;
	if (!*lst)
	{
		*lst = new_token;
		return ;
	}
	current_node = *lst;
	while (current_node && current_node->next)
		current_node = current_node->next;
	current_node->next = new_token;
	new_token->prev = current_node;
}

/**
 * @brief Append a separator token to the token list
 *
 * @param type Type of separator token
 * @param ptr_line Pointer to the current position in the input line
 * @param list Pointer to the token list
 * @return int 1 for success, 0 for failure
 */
int	append_separator(t_token_type type, char **ptr_line, t_token **list)
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

/**
 * @brief Handle separator token and add to token list
 *
 * @param ptr_line Pointer to the current position in input
 * @param token_list Pointer to the token list
 * @return int 1 for success, 0 for failure
 */
int	handle_separator(char **ptr_line, t_token **token_list)
{
	t_token_type	token_type;

	token_type = determine_separator_type(*ptr_line);
	return (append_separator(token_type, ptr_line, token_list) && 1);
}

/**
 * @brief Append an identifier token to the token list
 *
 * @param shell Shell structure for error reporting
 * @param ptr_line Pointer to current position in input
 * @param token_lst Pointer to token list
 * @return int 1 for success, 0 for failure
 */
int	append_identifier(t_shell *shell, char **ptr_line, t_token **token_lst)
{
	char	*tmp_line;
	char	*value;
	t_token	*token;
	size_t	len;

	tmp_line = *ptr_line;
	len = get_token_length(tmp_line, shell);
	if (len == 0)
		return (0);
	value = ft_substr(tmp_line, 0, len);
	if (!value)
		return (0);
	token = ft_create_new_token(value, TOKEN_IDENTIFIER);
	if (!token)
	{
		free(value);
		return (0);
	}
	*ptr_line += len;
	ft_token_list_add_back(token_lst, token);
	return (1);
}
