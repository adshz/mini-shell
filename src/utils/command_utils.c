/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:56:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 21:54:08 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "libft.h"
#include <stdlib.h>

void	free_cmd(void *content)
{
	t_ast_node	*cmd;

	if (!content)
		return ;
	cmd = (t_ast_node *)content;
	free_ast(cmd);
}

void	add_command_to_history(t_shell *shell, t_ast_node *cmd)
{
	t_list	*new_cmd;

	if (!shell || !cmd)
		return ;
	new_cmd = ft_lstnew(cmd);
	if (!new_cmd)
		return ;
	ft_lstadd_back(&shell->cmds, new_cmd);
	shell->ast = NULL;
}
