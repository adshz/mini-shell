/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 state_handler.h									:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/18 02:15:20 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/18 02:15:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */

#ifndef STATE_HANDLER_H
# define STATE_HANDLER_H

# include "lexer_types.h"

/* State Management */
/**
 * @brief Determines next tokenizer state
 * @param current_state Current state
 * @param c Character being processed
 * @return Next tokenizer state
 */
t_tokeniser_state	get_next_state(t_tokeniser_state current_state, char c);

/**
 * @brief Calculates length of next token
 * @param input String to analyze
 * @return Length of next token
 */
size_t				get_token_length_with_state(const char *input);

/* State Transitions */
/**
 * @brief Handles state transitions from normal state
 * @param c Character being processed
 * @return Next tokenizer state
 */
t_tokeniser_state	handle_normal_state(char c);

/**
 * @brief Handles state transitions from double quote state
 * @param c Character being processed
 * @param was_backslash Pointer to backslash flag
 * @return Next tokenizer state
 */
t_tokeniser_state	handle_double_quote_state(char c, int *was_backslash);

#endif 
