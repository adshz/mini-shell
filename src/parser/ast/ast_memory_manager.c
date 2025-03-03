/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_memory_manager.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 08:28:43 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 08:29:30 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "parser/parser.h"

/* AST memory handling
 *
 * This file manages memory for the AST, including cleanup
 */
// Clean up AST nodes recursively
void	recursively_ft_cleanup_ast(t_shell *shell, t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		clean_cmd_node(node);
	else
	{
		if (node->left)
			recursively_ft_cleanup_ast(shell, node->left);
		if (node->right)
			recursively_ft_cleanup_ast(shell, node->right);
	}
	free(node);
}

// Clean up entire AST and related resources
void	ft_cleanup_ast(t_shell *shell, t_ast_node **ast)
{
	recursively_ft_cleanup_ast(shell, *ast);
	*ast = NULL;
	clean_token_lst(&shell->tokens);
}

// Clean up a command node's resources
void	clean_cmd_node(t_ast_node *node)
{
	if (!node)
		return ;
	cleanup_io_list(&(node->io_list));
	free(node->raw_command);
	free_char2(node->expanded_argv);
}

// Free 2D char array -> this can go to `utils` module since it is a common one
void	free_char2(char **tofree)
{
	size_t	i;

	if (!tofree)
		return ;
	i = 0;
	while (tofree[i])
		free(tofree[i++]);
	free(tofree);
}
