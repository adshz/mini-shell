/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:31:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:31:41 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COMMAND_EXECUTOR_H
# define COMMAND_EXECUTOR_H

int	execute_command_node(t_shell *shell, t_ast_node *node, bool is_pipe);
int	execute_external_cmd(t_shell *shell, t_ast_node *node);
#endif
