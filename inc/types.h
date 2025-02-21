/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 17:09:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/12 18:33:25 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <termios.h>
# include <sys/types.h>
# include "libft.h"
# include "lexer/lexer.h"
# include "hashtable/hashtable.h"

# define SIG_HEREDOC_MODE 1
# define SIG_HEREDOC_INT 2
# define SIG_NONE 0

/* Forward declarations */
struct						s_ast_node;
typedef struct s_ast_node	t_ast_node;

/* Heredoc data structure */
typedef struct s_heredoc_data
{
	char		*content_path;
	int			content_fd;
	const char	*delimiter;
}	t_heredoc_data;

/*########################################*/

/*
 * Environment Structure
*/
// typedef struct s_env
// {
// 	char	*key;
// 	char	*value;
// 	struct s_env	*next;
// }	t_env;

// use t_hash_item as t_env
typedef t_hash_item			t_env;

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
	char			*line;
	t_token			*tokens;
	t_ast_node		*ast;
	t_hashmap		*env;
	char			**env_array;
	bool			env_modified;
	t_list			*cmds;
	pid_t			pid;
	pid_t			*pids;
	char			*old_pwd;
	char			**history;
	int				exit_status;
	int				stdin_backup;
	int				stdout_backup;
	bool			heredoc_sigint;
	bool			signint_child;
	bool			in_pipe;
	int				in_heredoc;
	int				signal;
	struct termios	term_settings;
	bool			in_double_quotes;
	bool			in_single_quotes;
}	t_shell;

#endif
