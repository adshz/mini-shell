/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/23 15:57:11 by szhong            #+#    #+#             */
/*   Updated: 2025/01/23 15:58:32 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ERRORS_H
# define ERRORS_H
# define PROMPT "minishell$ "
# define SHELL_SUCCESS 0
# define SHELL_ERROR 1
# include "types.h"
/* Error codes */
# define ERR_NONE 0
# define ERR_MALLOC 12
# define ERR_SYNTAX 2
# define ERR_CMD_NOT_FOUND 127
# define ERR_NOT_EXECUTABLE 126
# define ERR_PERMISSION 126

/* Error messages */
# define MSG_MALLOC "Memory allocation failed"
# define MSG_SYNTAX "Syntax error"
# define MSG_CMD_NOT_FOUND "Command not found"
# define MSG_PERMISSION "Permission denied"
# define MSG_NO_SUCH_FILE "No such file or directory"
# define MSG_FORK	"Fork Failed"

/* Errors Types */
/**
 * @brief Error types enumeration for shell erro handling
 *
 * Defines all possible error types that could occur during shell operation
 * Used for consistent error reporting and handling across the program.
 *
 * @param ERR_NONE			No error occurred (SUCCESS)
 * @param ERR_MALLOC		Memory allocation failure
 * @param ERR_SYNTAX		Command syntax error
 * @param ERR_CMD_NOT_FOUND	Command not found in PATH variable
 * @param ERR_PERMISSION	Permission denied accessing file/directory
 * @param ERR_FILE_NOT_FOUND	File not found
 * @param ERR_PWD_ACCESS	Cannot access or determine working directory
 * @param ERR_PIPE			Pipe creation or handling failure
 * @param ERR_FORK			Process forking failure
 * @param ERR_DUP			File descriptor duplication failure
 * @param ERR_EXEC			Command execution failure
 * @param ERR_ENV			Environment variable operation failure
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
void    handle_error(t_shell *shell, t_error_type error, const char *detail);
int     set_error_status(t_shell *shell, int status);
int     print_error(char *cmd, char *msg, int error_code);
#endif
