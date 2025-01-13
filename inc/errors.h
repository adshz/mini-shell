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

/* Basic status codes */
# define ERROR 1
# define SUCCESS 0

/* Legacy error codes - maintain for compatibility */
# define ERR_GENERAL 1
# define ERR_MALLOC 1
# define ERR_SYNTAX 2
# define ERR_CMD_NOT_FOUND 127
# define ERR_PERMISSION 126
# define ERR_NOT_EXECUTABLE 126
# define ERR_NO_SUCH_FILE 127

/* Legacy error messages - maintain for compatibility */
# define MSG_MALLOC "Memory allocation failed"
# define MSG_SYNTAX "Syntax error"
# define MSG_CMD_NOT_FOUND "Command not found"
# define MSG_PERMISSION "Permission denied"
# define MSG_NOT_EXECUTABLE "Not executable"
# define MSG_NO_SUCH_FILE "No such file or directory"
# define MSG_FORK "Fork failed"

/* New error type enum for future use */
typedef enum e_error_type
{
	ERR_NONE,
	ERR_MALLOC_NEW,
	ERR_SYNTAX_NEW,
	ERR_CMD_NOT_FOUND_NEW,
	ERR_PERMISSION_NEW,
	ERR_FILE_NOT_FOUND,
	ERR_PIPE,
	ERR_FORK,
	ERR_DUP,
	ERR_EXEC
}	t_error_type;

/* Legacy error handling function - maintain for compatibility */
int		print_error(char *cmd, char *msg, int error_code);

/* New error handling functions */
void	print_error_new(t_error_type error, const char *detail);
void	handle_error(t_shell *shell, t_error_type error, const char *detail);
int		set_error_status(t_shell *shell, int status);

#endif


