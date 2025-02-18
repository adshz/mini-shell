/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:16:10 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:16:13 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SIGNAL_HANDLER_H
# define SIGNAL_HANDLER_H

/* Signal handling */
void	setup_child_process(void);
void	setup_parent_process(void);
int		handle_signal_termination(t_shell *shell, int status);
int		handle_signal_interrupt(t_shell *shell);

/* Status checking */
int		exited_normally(int status);
int		get_signal_from_status(int status);
int		get_exit_status(int status);
int		was_signaled(int status);

#endif 
