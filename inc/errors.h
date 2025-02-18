/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:57:11 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 18:22:22 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include "types.h"

/* Shell status codes */
# define PROMPT "minishell$ "
# define SHELL_SUCCESS 0
# define SHELL_ERROR 1

/* Error codes */
# define ERR_NONE 0
# define ERR_MALLOC 12
# define ERR_SYNTAX 2
# define ERR_CMD_NOT_FOUND 127
# define ERR_NOT_FOUND 127
# define ERR_NOT_EXECUTABLE 126
# define ERR_PERMISSION 126
# define ERR_IS_DIR 126
# define ERR_INVALID_ARG 1

/* Permission error messages */
# define MSG_PERMISSION "Permission denied"
# define ERR_MSG_PERMISSION "Permission denied"
# define MSG_NOT_EXECUTABLE "Permission denied"
# define ERR_MSG_NOT_EXECUTABLE "Permission denied"
# define MSG_NO_ACCESS "Permission denied"
# define ERR_MSG_NO_ACCESS "Permission denied"
# define MSG_IS_DIR "Is a directory"
# define ERR_MSG_IS_DIR "Is a directory"

/* Command error messages */
# define MSG_CMD_NOT_FOUND "Command not found"
# define ERR_MSG_CMD_NOT_FOUND "Command not found"
# define MSG_NO_SUCH_CMD "No such command"
# define ERR_MSG_NO_SUCH_CMD "No such command"
# define MSG_INVALID_CMD "Invalid command"
# define ERR_MSG_INVALID_CMD "Invalid command"

/* File error messages */
# define MSG_NO_SUCH_FILE "No such file or directory"
# define ERR_MSG_NO_SUCH_FILE "No such file or directory"
# define MSG_FILE_EXISTS "File exists"
# define ERR_MSG_FILE_EXISTS "File exists"
# define MSG_FILE_TOO_LONG "File name too long"
# define ERR_MSG_FILE_TOO_LONG "File name too long"
# define MSG_FILE_BUSY "File busy"
# define ERR_MSG_FILE_BUSY "File busy"

/* System error messages */
# define MSG_MALLOC "Memory allocation failed"
# define ERR_MSG_MALLOC "Memory allocation failed"
# define MSG_FORK "Fork failed"
# define ERR_MSG_FORK "Fork failed"
# define MSG_PIPE "Pipe failed"
# define ERR_MSG_PIPE "Pipe failed"
# define MSG_DUP "Dup failed"
# define ERR_MSG_DUP "Dup failed"

/* Syntax error messages */
# define MSG_SYNTAX "Syntax error"
# define ERR_MSG_SYNTAX "Syntax error"
# define MSG_UNEXPECTED_TOKEN "Unexpected token"
# define ERR_MSG_UNEXPECTED_TOKEN "Unexpected token"
# define MSG_MISSING_QUOTE "Missing quote"
# define ERR_MSG_MISSING_QUOTE "Missing quote"
# define MSG_INVALID_IDENTIFIER "Invalid identifier"
# define ERR_MSG_INVALID_IDENTIFIER "Invalid identifier"

/* Environment error messages */
# define MSG_ENV_NOT_SET "Environment not set"
# define ERR_MSG_ENV_NOT_SET "Environment not set"
# define MSG_PATH_NOT_SET "PATH not set"
# define ERR_MSG_PATH_NOT_SET "PATH not set"
# define MSG_HOME_NOT_SET "HOME not set"
# define ERR_MSG_HOME_NOT_SET "HOME not set"
# define MSG_INVALID_ENV "Invalid environment variable"
# define ERR_MSG_INVALID_ENV "Invalid environment variable"

/* Errors Types */
/**
 * @brief Error types enumeration for shell error handling
 *
 * Defines all possible error types that could occur during shell operation
 * Used for consistent error reporting and handling across the program.
 *
 * @param ERR_NONE            No error occurred (SUCCESS)
 * @param ERR_MALLOC         Memory allocation failure
 * @param ERR_SYNTAX         Command syntax error
 * @param ERR_CMD_NOT_FOUND  Command not found in PATH variable
 * @param ERR_PERMISSION     Permission denied accessing file/directory
 * @param ERR_FILE_NOT_FOUND File not found
 * @param ERR_PWD_ACCESS     Cannot access or determine working directory
 * @param ERR_PIPE          Pipe creation or handling failure
 * @param ERR_FORK          Process forking failure
 * @param ERR_DUP           File descriptor duplication failure
 * @param ERR_EXEC          Command execution failure
 * @param ERR_ENV           Environment variable operation failure
*/
typedef enum e_rror_type
{
	ERROR_NONE,
	ERROR_MALLOC,
	ERROR_SYNTAX,
	ERROR_CMD_NOT_FOUND,
	ERROR_PERMISSION,
	ERROR_FILE_NOT_FOUND,
	ERROR_PWD_ACCESS,
	ERROR_PIPE,
	ERROR_FORK,
	ERROR_DUP,
	ERROR_EXEC,
	ERROR_ENV
}	t_error_type;

/* Error handling functions */
void	handle_error(t_shell *shell, t_error_type error, const char *detail);
int		set_error_status(t_shell *shell, int status);
int		print_error(char *cmd, char *msg, int error_code);

#endif
