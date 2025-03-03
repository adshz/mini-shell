/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:57:11 by szhong            #+#    #+#             */
/*   Updated: 2025/02/22 11:51:21 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "types.h"
# include <unistd.h>

/* Shell status codes */
# define PROMPT "minishell$ "
# define SHELL_SUCCESS 0
# define SHELL_ERROR 1
# define NOT_EXIT 0
# define IS_EXIT 1

enum e_shell_err
{
	FAILURE,
	ERROR,
	INV_ARGS,
	HASH_FULL,
};
#endif
