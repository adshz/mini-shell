/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:49:14 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 21:49:48 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_UTILS_H
# define BUILTIN_UTILS_H

# include <stdbool.h>
# include "shell.h"
# include "libft.h"

/* String manipulation */
const char	*skip_quotes(const char *str);

/* Character checking */
bool		check_first_char(const char *start);

/* Identifier validation */
bool		is_valid_identifier(const char *str);

#endif 