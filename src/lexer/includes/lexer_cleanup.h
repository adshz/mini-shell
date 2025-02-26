/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_cleanup.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:15:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:15:50 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_CLEANUP_H
# define LEXER_CLEANUP_H

void	ft_free_token(t_token *token);
void	ft_clean_token_list(t_token **list);

#endif
