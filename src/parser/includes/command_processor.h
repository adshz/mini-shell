/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:39:29 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:40:10 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COMMAND_PROCESSOR_H
# define COMMAND_PROCESSOR_H

// Command processing
t_ast_node	*ft_build_command_subtree(t_shell *shell);
bool		ft_build_raw_command_string(t_shell *shell, char **raw_cmd_ptr);

#endif

