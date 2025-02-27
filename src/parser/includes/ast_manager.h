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
# include "../parser.h"
// Node creation and manipulation
t_ast_node		*ft_create_new_node(t_ast_node_type type);
t_ast_node		*ft_combine_node(t_shell *shell, t_token_type operator, \
							t_ast_node *left, t_ast_node *right);
t_ast_node_type	ft_get_ast_node_type(t_token_type type);
// Memory management
void			ft_cleanup_ast(t_shell *shell, t_ast_node **ast);
void			clean_cmd_node(t_ast_node *node);
void			recursively_ft_cleanup_ast(t_ast_node *node);
void			cleanup_io_list(t_io_node **list);
void			free_char2(char **tofree);

#endif 
