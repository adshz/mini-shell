/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_handler.h                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 18:42:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:41:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECTION_HANDLER_H
# define REDIRECTION_HANDLER_H

# include "parser_types.h"
// Redirection processing
bool		ft_process_redirection(t_shell *shell, t_io_node **io_lst);
bool	ft_is_redirection(t_token_type type);
t_io_node	*ft_create_io_node(t_token_type type, char *value);
void	    cleanup_io_list(t_io_node	**lst);


#endif
