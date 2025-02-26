/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_processor.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:34:31 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:35:12 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/* Redirection Token Processing
 *
 * This file processes redirection tokens in commands
 *
 **/
// Process redirection tokens in a command
bool ft_process_redirection(t_shell *shell, t_io_node **io_lst);

// Check if token is a redirection
bool ft_is_redirection(t_token_type type);

// Convert token type to IO type
t_io_type convert_to_io_type(t_token_type type);



