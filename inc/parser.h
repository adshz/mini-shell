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

# ifndef PARSER_H
# define PARSER_H

# include <stdlib.h>
# include "lexer.h"
# include "types.h"
# include "shell.h"

// Function declarations
t_ast_node	*parse(t_token *tokens, t_shell *shell);
t_ast_node	*parse_expression(t_token **tokens, t_shell *shell);
t_ast_node	*handle_redirections(t_ast_node *node, t_token **tokens, t_shell *shell);
t_ast_node	*handle_redirection(t_ast_node *node, t_token **tokens);
t_ast_node	*parse_pipeline(t_token **tokens, t_shell *shell);
t_ast_node	*parse_command(t_token **tokens);
t_ast_node	*parse_simple_command(t_token **tokens);
t_ast_node	*parse_argument(t_token **tokens);

// Utility functions
bool		is_redirection_token(t_token_type type);
bool		is_variable_token(t_token_type type);
bool		is_word_token(t_token_type type);
bool		is_quote_token(t_token_type type);

// AST node functions
t_ast_node	*create_ast_node(t_ast_type type, char *value);
void		free_ast(t_ast_node *node);

#endif
