/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:36:49 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:37:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPE_EXECUTOR_H
# define PIPE_EXECUTOR_H

void	execute_pipe_child(t_shell *shell, t_ast_node *node, \
							int pipe_ends[2], t_pipe_role role);
int	execute_pipeline(t_shell *shell, t_ast_node *ast_tree);
#endif
