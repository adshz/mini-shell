/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:22:08 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:23:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Parser State Management
 *
 * The file manages the state and handles errors
 *
 */
// Advance to next token during parsing
void ft_get_next_token(t_shell *shell);

// Set parser error state
void set_parse_err(t_shell *shell, t_parse_err_type type);

// Clean up after parse errors
void cleanup_current_command(t_shell *shell); // might delete it


