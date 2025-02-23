/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_matrixfree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 11:13:34 by szhong            #+#    #+#             */
/*   Updated: 2025/02/22 11:46:47 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "utils.h"

void	ft_matrixfree(char ***matrix)
{
	int	i;

	i = 0;
	while (matrix && matrix[0] && matrix[0][i])
	{
		free(matrix[0][i]);
		i++;
	}
	if (matrix && *matrix)
	{
		free(*matrix);
		*matrix = NULL;
	}
}
