/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:06:52 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:08:53 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

static void	recursively_ft_cleanup_ast(t_ast_node *node);
void		ft_cleanup_ast(t_shell *shell, t_ast_node **ast);

