/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 14:44:07 by szhong            #+#    #+#             */
/*   Updated: 2024/11/11 13:48:06 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef SHELL_H
# define SHELL_H
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/types.h>
# include "libft.h"
# include <signal.h>
# include <stdlib.h>

typedef struct s_command
{
	char	*name;	//Command name
	char	**args; //Command arguments
	int		pipe_in; // Input pipe
	int		pipe_out; // Output pipe
	int		type; // BUILDIN, EXTERNAL, OR INVALID
	t_builtin_table	*builtins; //builtin commands table
}	t_command;

typedef struct s_shell
{
	t_hashmap	*env; // Environment variable using hash table as DS
	t_list		*history; // linked list for command history
	int			last_status; // Last command's exit status
	int			running; // whether shell is running
	pid_t		shell_pid  // Shell's process ID
}	t_shell;

typedef struct s_builtin_table
{
	t_hashmap	*commands;
	int			count;
}	t_builtin_table;

typedef struct s_hash_item
{
	char	*key; // key string
	void	*value; // generic value pointer (can store function pointers)
	struct	s_hash_item	*next; // for chainning
}	t_hash_item;

typedef struct s_hashmap
{
	t_hash_item	**item; // array of hash item pointers
	unsigned int	size;
	unsigned int	count;
}	t_hashmap;

// TODO
//typedefenum {
//    ERR_NONE,
//    ERR_FILE,      // For file operations
//    ERR_MEMORY     // For malloc fails
//} t_error_type;

// TODO
//void  handle_error(t_error_type type, const char *msg);
char*	ft_pwd(void);

#endif
