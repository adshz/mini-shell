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
# include "types.h"
# include "lexer/lexer.h"

/**
 * @brief Structure for parsing redirection tokens
 * 
 * This structure contains the current token, the result left node,
 * the previous token type, and the shell.
 * @example
 * we have `echo hello > file.txt`
 * the result_left_node will be the command `echo hello`
 * the current will be the redirection token `>`
 * the prev_type will be the type of the token before the redirection token
 * in the case of `echo hello > file.txt` the prev_type will be TOKEN_WORD
 * the shell will be the shell
 */
typedef struct s_redir_parse
{
	t_token			*current;
	t_ast_node		*result_left_node;
	t_token_type	prev_type;
	t_shell			*shell;
}	t_redir_parse;

/**
 * @brief Structure for parsing redirection tokens
 * 
 * This structure contains the current token, the result left node,
 * the previous token type, and the shell.
 * t_token **current: the current token
 * t_token **tokens: the list of tokens of the command
 * t_token *next_token: single token, the token after the redirection token 
 * t_ast_node *result_left_node: the command before the redirection token
 * t_ast_node *output_node: the output/target of the redirection token
 * t_token_type *prev_type: the type of the token before the redirection token
 * t_shell *shell: the shell
 * @example
 * we have `echo hello > file.txt`
 * the current will be the redirection token `>`
 * the tokens will be the tokens of the command `echo hello > file.txt`
 * the next_token will be the token after the redirection token `>` \
 * which is `file.txt`
 * the result (result_left_node) is 
 * to store the result of the command `echo hello`
 * the output_node will be the result of the command `echo hello > file.txt`
 * the prev_type will be the type of the token before the redirection token
 * which is TOKEN_WORD in our case of `echo hello > file.txt`
 */
typedef struct s_redir_token
{
	t_token_type	prev_type;
	t_token			**current;
	t_token			**tokens;
	t_token			*next_token;
	t_ast_node		*result_left_node;
	t_ast_node		*output_node;
	t_shell			*shell;
}	t_redir_token;

typedef struct s_word_token
{
	t_token_type	*prev_type;
	t_token			**current;
	t_token			**tokens;
	t_ast_node		*result;
	t_shell			*shell;
	t_ast_node		*cmd_node;
}	t_word_token;

/* Abstract Syntax Tree Node Types */
/**
 * @brief AST Node types for the shell's command parser
 *
 * Defines all possible types of nodes in the Abstract Syntax Tree
 * Used to represent different command components and operators
 *
 * @param AST_COMMAND		 Simple command node (e.g., "ls", "echo" etc)
 * @param AST_PIPE			Pipe operator node (|)
 * @param AST_REDIR_IN		Input redirection node (<)
 * @param AST_REDIR_OUT		Output redirection node (>)
 * @param AST_REDIR_APPEND	Append redirection node (>>)
 * @param AST_HEREDOC		Here document node (<<)
 * @param AST_AND			Logical AND operator node (&&)
 * @param AST_OR			Logical OR operator node (||)
 * @param AST_VAR_EXPANSION Variable expansion node ($VAR)
*/
typedef enum e_ast_type
{
	AST_COMMAND,
	AST_PIPE,
	AST_REDIR_IN,
	AST_REDIR_OUT,
	AST_REDIR_APPEND,
	AST_HEREDOC,
	AST_AND,
	AST_OR,
	AST_VAR_EXPANSION
}	t_ast_type;

/* AST Node Structure */
/**
 * @brief Abstract Syntax Tree Node Structure
 *
 * Represents a node in the AST, containing command information
 * variable expansion status, and binary tree links
 *
 * @param type			Node type (AST_COMMAND, AST_PIPE, AST_REDIR_IN, etc.)
 * @param value			Primary value (command name or filename for redirections)
 * @param args			Array of command arguments, NULL terminated
 * @param is_expanded	Flag indicating if node contains expanded variables
 * @param original		Original string before variable expansions
 * @param left			Left child node (e.g., represents command before pipe)
 * @param right			Right child node (e.g, represents command after pipe)
 * @param data			For storing heredoc data
*/
typedef struct s_ast_node
{
	t_ast_type			type;
	char				*value;
	char				**args;
	int					is_expanded;
	char				*original;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
	t_heredoc_data		data;
}	t_ast_node;

typedef struct s_heredoc_chain
{
	t_token		**tokens;
	t_token		*current;
	t_token		*delimiter;
	t_ast_node	*first_redir;
	t_ast_node	*last_redir;
	t_ast_node	*cmd_node;
	t_shell		*shell;
	int			total_heredocs;
	int			processed_count;
}	t_heredoc_chain;

#endif 
