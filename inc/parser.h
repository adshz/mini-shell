/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:19 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:50:19 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include "lexer.h"
# include "types.h"

typedef enum e_ast_type
{
    AST_COMMAND,     // Simple command
    AST_PIPE,        // Pipeline
    AST_REDIR_IN,    // Input redirection (<)
    AST_REDIR_OUT,   // Output redirection (>)
    AST_REDIR_APPEND,// Append redirection (>>)
    AST_HEREDOC,     // Here document (<<)
    AST_AND,         // AND operator (&&)
    AST_OR,          // OR operator (||)
    AST_VAR_EXPANSION // Variable expansion ($var)
} t_ast_type;

typedef struct s_ast_node
{
    t_ast_type          type;
    char                *value;      // For commands and file names
    char                **args;      // For command arguments
    int                 is_expanded; // Flag to indicate if this node contains expanded variables
    char                *original;   // Original unexpanded value (for variable expansions)
    struct s_ast_node   *left;
    struct s_ast_node   *right;
} t_ast_node;

// Parser functions
t_ast_node  *parse(t_token *tokens);
t_ast_node  *create_ast_node(t_ast_type type, char *value);
void        free_ast(t_ast_node *node);
t_ast_node  *parse_command(t_token **tokens);
t_ast_node  *parse_pipeline(t_token **tokens);
t_ast_node  *parse_expression(t_token **tokens);
int         is_redirection_token(t_token_type type);
t_ast_node  *handle_redirection(t_ast_node *left, t_token **tokens);
t_ast_type  get_ast_type(t_token_type type);
t_ast_node	*handle_redirections(t_ast_node *node, t_token **tokens);
// New helper functions for variable expansion
int         is_variable_token(const char *token);
char        *expand_variable(t_shell *shell, const char *var_name);
t_ast_node  *create_expanded_node(t_shell *shell, const char *value);
int is_variable_token(const char *token);
#endif
