/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_core.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:44:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 23:52:26 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSE_UTILS_H
# define PARSE_UTILS_H
# include "parser_types.h"
// Main entry point for parsing tokens into an AST
t_ast_node	*parse(t_shell *shell, t_token *tokens_lst);

// Set parser error state
void		set_parse_err(t_shell *shell, t_parse_err_type err_type);

#endif
