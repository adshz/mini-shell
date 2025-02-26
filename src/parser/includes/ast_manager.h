/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_manager.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:45:16 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:40:54 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AST_MANAGER_H
# define AST_MANAGER_H
# include "parser_types.h"

t_ast_node		*ft_create_new_node(t_ast_node_type type);
t_ast_node		*ft_combine_node(t_shell *shell, t_token_type operator, \
							t_ast_node *left, t_ast_node *right);
t_ast_node_type	ft_get_ast_node_type(t_token_type type);
void			ft_cleanup_ast(t_shell *shell, t_ast_node **ast);

#endif 
