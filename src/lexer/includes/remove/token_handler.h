/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 token_handler.h									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/18 02:15:20 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/18 02:15:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#ifndef TOKEN_HANDLER_H
# define TOKEN_HANDLER_H

# include "lexer_types.h"

/**
 * @brief Gets token type from token value
 * @param value String to analyze
 * @return Appropriate token type
 */
t_token_type	get_token_type(const char *value);
/* Token Creation and Management */
/**
 * @brief Creates new token with given type and value
 * @param type Token type to set
 * @param value String value for token
 * @return New token or NULL on error
 */
t_token			*create_token(t_token_type type, const char *value);

/**
 * @brief Frees token list and associated memory
 * @param head Head of token list to free
 */
void			free_tokens(t_token *head);

/* Token Type Handling */
/**
 * @brief Determines if character is a shell special character
 * @param c Character to check
 * @return 1 if special character, 0 otherwise
 */
int				is_special_char(char c);
/**
 * @brief Determines if string is an operator
 * @param str String to check
 * @return 1 if operator, 0 otherwise
 */
int				is_operator(const char *str);

/* Token Extraction */
/**
 * @brief Extracts token from input string
 * @param input Input string
 * @param len Length to extract
 * @param in_single_quotes Pointer to store if token is in single quotes
 * @return New string containing token or NULL on error
 */
char			*extract_token(const char *input, size_t len, \
					bool *in_single_quotes);
/**
 * @brief Advances input pointer past whitespace characters
 * @param input String to process
 * @return Pointer to first non-whitespace character
 */
const char		*skip_whitespace(const char *input);

#endif 
