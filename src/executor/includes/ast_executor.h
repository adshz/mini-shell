/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_executor.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:33:38 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:33:58 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef AST_EXECUTOR_H
# define AST_EXECUTOR_H

int	execute_ast_node(t_shell *shell, t_ast_node *ast_tree, bool is_pipe);
int	execute_and_node(t_shell *shell, t_ast_node *node);
int	execute_or_node(t_shell *shell, t_ast_node *node);
#endif
