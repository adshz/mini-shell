/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:49:24 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:33:54 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdbool.h>
# include "libft.h"
# include "types.h"
# include "lexer/lexer.h"  /* For t_token type */

/* Abstract Syntax Tree Node Types */
/**
 * @brief AST Node types for the shell's command parser
 *
 * Defines all possible types of nodes in the Abstract Syntax Tree
 * Used to represent different command components and operators
 *
 * @param AST_COMMAND		Simple command node (e.g., "ls", "echo" etc)
 * @param AST_PIPE			Pipe operator node (|)
 * @param AST_REDIR_IN		Input redirection node (<)
 * @param AST_REDIR_OUT		Output redirection node (>)
 * @param AST_REDIR_APPEND	Append redirection node (>>)
 * @param AST_HEREDOC		Here document node (<<)
 * @param AST_AND			Logical AND operator node (&&)
 * @param AST_OR			Logical OR operator node (||)
 * @param AST_VAR_EXPANSION	Variable expansion node ($VAR)
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
 * @brief Abstract Syntax Tree Node Structutre
 *
 * Represents a node in the AST, containing command information
 * variable expansion status, and binary tree links
 *
 * @param type		Node type (AST_COMMAND, AST_PIPE, AST_REDIR_IN, etc.)
 * @param value		Primary value (command name or filename for redirections)
 * @param args		Array of command arguments, NULL terminated
 * @param is_expanded	Flag indicating if node contains epanded variables
 * @param original		Original string before variable expansions
 * @param left			Left child node (e.g., represents command before pipe)
 * @param right			Right child node (e.g, represents command after pipe)
 * @param data		For storing heredoc data
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
	t_heredoc_data		data;  // For storing heredoc data
}	t_ast_node;

/* Parser functions */
t_ast_node	*parse(t_token *tokens, t_shell *shell);
t_ast_node	*parse_pipeline(t_token **tokens, t_shell *shell);
t_ast_node	*parse_expression(t_token **tokens, t_shell *shell);

/* Token type checking */
int			is_redirection_token(t_token_type type);

/* AST node functions */
void		free_ast(t_ast_node *ast);
t_ast_node	*create_ast_node(t_ast_type type, char *value);
bool		is_valid_ast(void *ast);

/* Command parsing */
t_ast_node	*parse_command(t_token **tokens, t_shell *shell);
void		*parse_redirection(void *tokens, t_shell *shell);
t_ast_node	*process_redirection(t_token *token, t_ast_node *result);

/* Redirection handling */
t_ast_node	*handle_redirections(t_ast_node *node,
				t_token **tokens, t_shell *shell);
t_ast_node	*handle_redirection(t_ast_node *node, t_token **tokens);

/* Command initialization helpers */
char		**allocate_args(int arg_count);
int			is_variable_token(const char *value);
t_ast_node	*create_default_heredoc_command(void);
bool		init_command_expansion(t_ast_node *node, const char *value);

/* Command construction helpers */
t_ast_node	*parse_redirection_construct(t_ast_node *cmd_node,
				t_token **tokens, t_shell *shell);
t_ast_node	*process_consecutive_redirections(t_ast_node *node,
				t_token **tokens, t_shell *shell);
int			count_args(t_token *start);
t_ast_node	*init_command_node(t_token *start, int arg_count);
int			fill_args(char **args, t_token *start, int arg_count);
void		skip_processed_args(t_token **tokens, int count);

/* Redirection and command node functions */
t_ast_node	*create_redirection_node(t_token_type type, const char *file_value);
t_ast_node	*create_command_from_token(t_token *token);
t_ast_node	*attach_command_node(t_ast_node *result, t_ast_node *cmd_node);

/* Main command parser function */
t_ast_node	*parse_command(t_token **tokens, t_shell *shell);

/* Redirection handler functions */
int			is_redirection_token(t_token_type type);
t_ast_node	*process_initial_redirection(t_token **tokens, t_shell *shell);
t_ast_node	*process_final_redirection(t_ast_node *node, t_token **tokens, \
									t_shell *shell);

/* Heredoc handler functions */
t_ast_node	*handle_heredoc_command(t_token **tokens, t_shell *shell);

/* Command node functions */
t_ast_node	*create_command_from_word(t_token *word_token);
t_ast_node	*create_regular_command(t_token **tokens, t_token *start, \
								int arg_count);
t_token		*find_last_word_token(t_token *start);
int			count_command_args(t_token *tokens);

#endif