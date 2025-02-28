/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_number_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 21:35:31 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 21:36:30 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./echo.h"

static void	handle_negative_number(int *n, char *buf, int *i, int *is_negative)
{
	*is_negative = 0;
	if (*n < 0)
	{
		*is_negative = 1;
		buf[(*i)++] = '-';
		*n = -(*n);
	}
}

static void	convert_digits(int n, char *buf, int *i)
{
	if (n == 0)
		buf[(*i)++] = '0';
	while (n > 0)
	{
		buf[(*i)++] = '0' + (n % 10);
		n /= 10;
	}
	buf[*i] = '\0';
}

static void	reverse_digits(char *buf, int start, int end)
{
	char	tmp;

	while (start < end)
	{
		tmp = buf[start];
		buf[start] = buf[end];
		buf[end] = tmp;
		start++;
		end--;
	}
}

void	ft_itoa_buf(int n, char *buf)
{
	int	i;
	int	is_negative;
	int	start_index;

	i = 0;
	handle_negative_number(&n, buf, &i, &is_negative);
	convert_digits(n, buf, &i);
	start_index = 0;
	if (is_negative)
		start_index = 1;
	reverse_digits(buf, start_index, i - 1);
}
