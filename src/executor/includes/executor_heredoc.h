/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:28:06 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:28:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_HEREDOC_H
# define EXECUTOR_HEREDOC_H
# define HEREDOC_CTRL_C 130
# define NORMAL_OPERATION 0
# define EXIT_NORMAL 0
# define HEREDOC_CTRL_D 0
# include "shell_forward.h"

int		heredoc_expand_var_to_fd(t_shell *shell, char *str, size_t i, int fd);
void	heredoc_expander(t_shell *shell, char *str, int fd);
void	heredoc_handler(t_shell *shell, t_io_node *io, int fd[2]);
bool	traverse_expand_tree(t_shell *shell, t_ast_node *ast_node);
bool	is_delimiter(char *delimiter, char *str);
#endif
