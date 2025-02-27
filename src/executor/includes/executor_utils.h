/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:39:48 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:42:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXECUTOR_UTILS_H
# define EXECUTOR_UTILS_H
# include "../executor.h"
int	exec_print_err(t_err err);
int	exec_get_exit_status(int status);
#endif
