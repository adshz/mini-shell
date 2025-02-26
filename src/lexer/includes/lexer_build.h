/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_build.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 02:19:07 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 02:19:52 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef LEXER_BUILD_H
# define LEXER_BUILD_H
# include "lexer_types.h"
# include "shell.h"

void	ft_token_list_add_back(t_token **lst, t_token *new_token);
int		append_separator(t_token_type type, char **ptr_line, t_token **list);
int		handle_separator(char **ptr_line, t_token **token_list);
int		append_identifier(t_shell *shell, char **ptr_line, t_token **token_lst);

#endif
