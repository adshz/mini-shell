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
void	recursively_ft_cleanup_ast(t_ast_node *node)
{
	if (!node)
		return ;
	if (node->type == NODE_CMD)
		clean_cmd_node(node);
	else
	{
		if (node->left)
			recursively_ft_cleanup_ast(node->left);
		if (node->right)
			recursively_ft_cleanup_ast(node->right);
	}
	free(node);
}

// Clean up entire AST and related resources
void	ft_cleanup_ast(t_shell *shell, t_ast_node **ast)
{
	recursively_ft_cleanup_ast(*ast);
	*ast = NULL;
	clean_token_lst(&shell->tokens);
}

// Clean up a command node's resources
void	clean_cmd_node(t_ast_node *node)
{
	if (!node)
		return ;
	ft_clear_io_list(&(node->io_list));
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

// Clean up token list
void	clean_token_lst(t_token **lst)
{
	t_token	*curr_token;
	t_token	*next;

	curr_token = *lst;
	if (!curr_token)
		return ;
	while (curr_token)
	{
		free(curr_token->value);
		next = curr_token->next;
		free(curr_token);
		curr_token = next;
	}
	*lst = NULL;
}


