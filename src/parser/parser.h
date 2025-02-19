/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parser.h											:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/18 02:15:20 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/18 02:15:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#ifndef PARSER_H
# define PARSER_H

# include "lexer/lexer.h"
# include "expander/expander.h"
# include "includes/parser_types.h"
# include "includes/ast.h"
# include "includes/redirection_handler.h"
# include "includes/pipeline_handler.h"
# include "includes/heredoc_handler.h"
# include "includes/command_parser.h"
# include "includes/parser_redirection.h"
# include "includes/parser_variable_expansion.h"
# include "includes/parser_expressions.h"

t_ast_node	*parse(t_token *tokens, t_shell *shell);
#endif
