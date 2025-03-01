/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:09:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:56:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <termios.h>
# include <sys/types.h>
# include "libft.h"
# include "parser/includes/parser_types.h"

// Forward declarations
typedef struct s_token t_token;
typedef struct s_ast_node t_ast_node;
typedef struct s_hashmap t_hashmap;
typedef struct s_parse_err t_parse_err;
typedef struct s_err t_err;
typedef struct s_path t_path;

/* Shell Structure */
/**
 * @brief Main shell structure holding program data
 * 
 * The struct contains all essential components for program operations
 * such as command parsing data, execution state, environement management
 * process tracking, and terminal settings.
 * 
 * @param line Current user command line being parsered by parser
 * @param tokens List of tokens by scanner and lexer (lexcial analysis)
 * @param ast				Abstract syntax tree representing commands, flags etc
 * @param env				Environment variables stored by Hash Table
 * @param env_array			Cached environment array
 * @param env_modified		Flag to track if env was modified
 * @param cmds				History of executed commands using linked list
 * @param pid				Process ID of the shell
 * @param pids				Array of child process IDs
 * @param old_pwd			Previous working directory path
 * @param history			Array of command history entries
 * @param exit_status		Exit status of the last executed command
 * @param stdin_backup		Backup of standard input file descriptor
 * @param stdout_backup		Backup of stadnard output file descriptor
 * @param term_settings		Terminal configuration settings
 * @param heredoc_sigint	Flag indicating heredoc interrupt state
 * @param signint_child		Flag indicating child process interrupt state
 * @param in_pipe			Flag indicating if currently in a pipeline
*/
typedef struct s_shell
{
	t_parse_err				parse_err;
	t_list					*mem_list;
	char					*line;
	t_token					*tokens;
	t_token					*curr_token;
	t_ast_node				*ast;
	t_hashmap				*env;
	char					**environ;
	int						stdin;
	int						stdout;
	int						exit_status;
	struct termios			original_term;
}	t_shell;

#endif
