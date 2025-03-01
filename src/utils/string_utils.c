/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 23:06:03 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 23:16:56 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils/utils.h"

bool	is_only_whitespace(const char *str)
{
	if (!str)
		return (true);
	while (*str)
	{
		if (!ft_isspace(*str))
			return (false);
		str++;
	}
	return (true);
}