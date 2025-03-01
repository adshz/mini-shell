/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parser_types.h										:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/18 02:15:10 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/18 02:15:13 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#ifndef PARSER_TYPES_H
# define PARSER_TYPES_H

# include <stdbool.h>
# include "libft.h"
# include "lexer/lexer.h"

typedef enum e_parse_err_type
{
	E_MEM = 1,
	E_SYNTAX
}	t_parse_err_type;

typedef enum e_io_type
{
	IO_IN,
	IO_OUT,
	IO_HEREDOC,
	IO_APPEND,
}	t_io_type;

typedef enum e_ast_type
{
	NODE_PIPE,
	NODE_AND,
	NODE_OR,
	NODE_CMD
}	t_ast_node_type;

typedef struct s_parse_err
{
    t_parse_err_type    type;
    char                *msg;
}   t_parse_err;

/* here_doc is file descriptor*/
typedef struct s_io_node
{
	t_io_type		type;
	char			*value;
	char			**expanded_value;
	int				here_doc;
	struct s_io_node	*prev;
	struct s_io_node	*next;
}	t_io_node;

typedef struct s_ast_node
{
	t_ast_node_type		type;
	t_io_node			*io_list;
	char				*raw_command;
	char				**expanded_argv;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

#endif 
