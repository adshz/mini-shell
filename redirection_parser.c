/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:49:40 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:53:25 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "lexer/lexer.h"
#include "shell.h"

/**
 * @brief Handles word token processing
 * @param current Current token
 * @param prev_type Previous token type
 * @param result Current result tree
 * @param tokens Token stream
 * @param shell Shell instance
 * @return Updated result tree or NULL on failure
 */
static t_ast_node	*handle_word_token(t_token **current, \
									t_token_type *prev_type, \
									t_ast_node *result, t_token **tokens, \
									t_shell *shell)
{
	t_ast_node	*cmd_node;

	ft_putstr_fd("\nDEBUG [handle_word_token]: Processing word token: [", STDERR_FILENO);
	ft_putstr_fd((*current)->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	if ((*current)->next && is_redirection_token((*current)->next->type))
	{
		ft_putstr_fd("DEBUG [handle_word_token]: Next token is redirection\n", STDERR_FILENO);
		*prev_type = (*current)->type;
		*current = (*current)->next;
		*tokens = *current;
		return (result);
	}
	if (is_redirection_token(*prev_type))
	{
		ft_putstr_fd("DEBUG [handle_word_token]: Previous token was redirection\n", STDERR_FILENO);
		*prev_type = (*current)->type;
		*current = (*current)->next;
		*tokens = *current;
		return (result);
	}
	cmd_node = create_command_from_token(*current);
	if (!cmd_node)
	{
		ft_putstr_fd("DEBUG [handle_word_token]: Failed to create command node\n", STDERR_FILENO);
		shell->exit_status = 258;
		return (NULL);
	}
	ft_putstr_fd("DEBUG [handle_word_token]: Created command node\n", STDERR_FILENO);
	result = attach_command_node(result, cmd_node);
	*prev_type = (*current)->type;
	*current = (*current)->next;
	*tokens = *current;
	return (result);
}

/**
 * @brief Handles redirection token processing
 * @param current Current token
 * @param prev_type Previous token type
 * @param result Current result tree
 * @param tokens Token stream
 * @param shell Shell instance
 * @return Updated result tree or NULL on failure
 */
static t_ast_node	*handle_redirection_token(t_token **current,
										t_token_type *prev_type,
										t_ast_node *result,
										t_token **tokens,
										t_shell *shell)
{
	t_ast_node	*new_result;
	t_token		*next_token;
	t_token		*cmd_token;

	ft_putstr_fd("\nDEBUG [handle_redirection_token]: Processing redirection token: [",
		STDERR_FILENO);
	ft_putstr_fd((*current)->value, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	if ((*current)->type == TOKEN_HEREDOC)
	{
		ft_putstr_fd("DEBUG [handle_redirection_token]: Found heredoc token\n",
			STDERR_FILENO);
		next_token = (*current)->next;
		if (!next_token || next_token->type != TOKEN_WORD)
		{
			shell->exit_status = 258;
			return (NULL);
		}
		if (!result)
		{
			cmd_token = next_token->next;
			while (cmd_token && !is_redirection_token(cmd_token->type))
				cmd_token = cmd_token->next;
			if (cmd_token && cmd_token->next
				&& cmd_token->next->type == TOKEN_WORD)
			{
				result = create_command_from_token(cmd_token->next);
				if (!result)
				{
					shell->exit_status = 258;
					return (NULL);
				}
			}
		}
		new_result = process_redirection(*current, result);
		if (!new_result)
		{
			ft_putstr_fd("DEBUG [handle_redirection_token]: Failed to process redirection\n", STDERR_FILENO);
			shell->exit_status = 258;
			return (NULL);
		}
		*prev_type = (*current)->type;
		if (next_token && next_token->next)
		{
			*tokens = next_token->next;
			*current = *tokens;
		}
		else
		{
			*tokens = NULL;
			*current = NULL;
		}
		return (new_result);
	}
	new_result = process_redirection(*current, result);
	if (!new_result)
	{
		ft_putstr_fd("DEBUG [handle_redirection_token]: Failed to process redirection\n",
			STDERR_FILENO);
		shell->exit_status = 258;
		return (NULL);
	}
	ft_putstr_fd("DEBUG [handle_redirection_token]: Successfully processed redirection\n",
		STDERR_FILENO);
	*prev_type = (*current)->type;
	*tokens = (*current)->next->next;
	*current = *tokens;
	return (new_result);
}

/**
 * @brief Parses a redirection construct and builds its AST
 * @param left Left subtree to attach
 * @param tokens Token stream
 * @param shell Shell instance
 * @return Complete redirection AST or NULL on failure
 * 
 * Builds an AST for a redirection construct, handling both the redirection
 * operator and its target (file or heredoc delimiter).
 */
t_ast_node	*parse_redirection_construct(t_ast_node *left, t_token **tokens, \
t_shell *shell)
{
	t_token			*current;
	t_ast_node		*result;
	t_token_type	prev_type;

	ft_putstr_fd("\nDEBUG [parse_redirection_construct]: Starting redirection parsing\n",
		STDERR_FILENO);
	current = *tokens;
	result = left;
	prev_type = TOKEN_EOF;
	while (current)
	{
		ft_putstr_fd("DEBUG [parse_redirection_construct]: Processing token: [",
			STDERR_FILENO);
		ft_putstr_fd(current->value, STDERR_FILENO);
		ft_putstr_fd("] of type ", STDERR_FILENO);
		ft_putnbr_fd(current->type, STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		if (current->type == TOKEN_WORD)
			result = handle_word_token(&current, &prev_type, result,
				tokens, shell);
		else if (is_redirection_token(current->type))
			result = handle_redirection_token(&current, &prev_type,
				result, tokens, shell);
		else
		{
			ft_putstr_fd("DEBUG [parse_redirection_construct]: Found non-redirection token, breaking\n",
				STDERR_FILENO);
			break ;
		}
		if (!result)
		{
			ft_putstr_fd("DEBUG [parse_redirection_construct]: Failed to process token\n",
				STDERR_FILENO);
			shell->exit_status = 258;
			return (NULL);
		}
	}
	ft_putstr_fd("DEBUG [parse_redirection_construct]: Finished redirection parsing\n",
		STDERR_FILENO);
	return (result);
}
