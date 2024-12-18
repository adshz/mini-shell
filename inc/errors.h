/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:47 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:56:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "shell.h"

/* Error codes */
# define ERR_MALLOC 1
# define ERR_SYNTAX 2
# define ERR_CMD_NOT_FOUND 127
# define ERR_PERMISSION 126
# define ERR_NOT_EXECUTABLE 126
# define ERR_NO_SUCH_FILE 127

/* Error messages */
# define MSG_MALLOC "Memory allocation failed"
# define MSG_SYNTAX "Syntax error"
# define MSG_CMD_NOT_FOUND "Command not found"
# define MSG_PERMISSION "Permission denied"
# define MSG_NOT_EXECUTABLE "Not executable"
# define MSG_NO_SUCH_FILE "No such file or directory"

/* Error handling functions */
int	print_error(char *cmd, char *msg, int error_code);

#endif


