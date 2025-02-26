/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_init.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:21:52 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:22:18 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_INIT_H
# define LEXER_INIT_H

int			init_lexer(t_shell *shell);
t_token		*ft_create_new_token(char *value, t_token_type type);
#endif
