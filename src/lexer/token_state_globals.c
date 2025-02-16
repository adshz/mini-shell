/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_state_globals.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 21:00:00 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 21:00:00 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "lexer/lexer.h"

/* Global variables for tracking token state during extraction */
bool g_token_in_single_quotes = false;
bool g_token_in_double_quotes = false;
t_tokeniser_state g_token_last_state = STATE_NORMAL; 