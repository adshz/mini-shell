/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_node_builder.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:31:39 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:32:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* Command Node Construction
 *
 * This file builds command nodes in the AST
 */
// Build a command subtree in the AST
t_ast_node *ft_build_command_subtree(t_shell *shell);
