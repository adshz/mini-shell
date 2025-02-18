/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_executor.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEREDOC_EXECUTOR_H
# define HEREDOC_EXECUTOR_H

# include "parser/parser.h"
int		setup_heredoc_pipe(t_ast_node *node);
int		collect_heredoc_content(t_ast_node *node, t_shell *shell);
#endif 