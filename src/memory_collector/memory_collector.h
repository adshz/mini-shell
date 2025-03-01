/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_collector.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 18:34:03 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 18:35:42 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MEMORY_COLLECTOR_H
# define MEMORY_COLLECTOR_H

# include "types.h"
# include <stdlib.h>
# include <stdbool.h>
# include "libft.h"

void	ft_delete_mem(void *ptr);
void	*ft_memory_delone(t_shell *shell, void *ptr);
void	*ft_memory_collector(t_shell *shell, void *ptr, bool clean_flag);
#endif
