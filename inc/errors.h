/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ERRORS_H
# define ERRORS_H

# include "shell.h"

# define ERROR 1
# define SUCCESS 0

typedef enum e_error_type
{
	ERR_NONE,
	ERR_MALLOC,
	ERR_SYNTAX,
	ERR_CMD_NOT_FOUND,
	ERR_PERMISSION,
	ERR_FILE_NOT_FOUND,
	ERR_PIPE,
	ERR_FORK,
	ERR_DUP,
	ERR_EXEC
}	t_error_type;

void	print_error(t_error_type error, const char *detail);
void	handle_error(t_shell *shell, t_error_type error, const char *detail);
int		set_error_status(t_shell *shell, int status);

#endif


