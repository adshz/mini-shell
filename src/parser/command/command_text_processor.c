/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_text_processor.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:32:52 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:34:02 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
/*
 * Command Text Processing
 *
 * This file processes command text from tokens
 *
 */
#include "parser/parser.h"

// Build raw command string from consecutive identifier tokens
bool ft_build_raw_command_string(t_shell *shell, char **raw_cmd_ptr);

// Join strings with a separatori // this could go to common utils
char *ft_strjoin3(const char *s1, const char *s2, char c);
