/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:38:04 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:38:24 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_REDIRECTION_H
# define EXECUTOR_REDIRECTION_H
# include "../executor.h"

t_err	ft_validate_read_perm(char *file);
int		ft_handle_input_redirection(t_io_node *io_lst, int *status);
t_err	ft_validate_write_perm(char *file);
int		ft_handle_output_redirection(t_io_node *io_lst, int *status);
int		ft_handle_append_redirection(t_io_node *io_lst, int *status);
void	reset_stds(t_shell *shell, bool is_pipe);
int	    check_redirection(t_ast_node *node);
#endif
