/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_expressions.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:41:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/19 18:41:54 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PARSER_EXPRESSIONS_H
# define PARSER_EXPRESSIONS_H
# include "types.h"

t_ast_node	*ft_parse_expression(t_shell *shell, int min_prec);
t_ast_node	*ft_parse_atomic_expression(t_shell *shell);
int			ft_precedence(t_token_type type);
int			curr_token_precd(t_shell *shell);
bool		is_binary_operator(t_shell *shell);

#endif
