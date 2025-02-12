/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_parser.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 12:49:40 by szhong            #+#    #+#             */
/*   Updated: 2025/01/28 12:52:47 by szhong           ###   ########.fr       */
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

	if ((*current)->next && is_redirection_token((*current)->next->type))
	{
		*prev_type = (*current)->type;
		*current = (*current)->next;
		*tokens = *current;
		return (result);
	}
	if (is_redirection_token(*prev_type))
	{
		*prev_type = (*current)->type;
		*current = (*current)->next;
		*tokens = *current;
		return (result);
	}
	cmd_node = create_command_from_token(*current);
	if (!cmd_node)
	{
		shell->exit_status = 258;
		return (NULL);
	}
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

	if ((*current)->type == TOKEN_HEREDOC)
	{
		next_token = (*current)->next;
		if (!next_token || next_token->type != TOKEN_WORD)
		{
			shell->exit_status = 258;
			return (NULL);
		}
		if (!result)
		{
			t_token *cmd_token = next_token->next;
			while (cmd_token && !is_redirection_token(cmd_token->type))
				cmd_token = cmd_token->next;
			if (cmd_token && cmd_token->next && cmd_token->next->type == TOKEN_WORD)
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
		return new_result;
	}
	new_result = process_redirection(*current, result);
	if (!new_result)
	{
		shell->exit_status = 258;
		return (NULL);
	}
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

	current = *tokens;
	result = left;
	prev_type = TOKEN_EOF;
	while (current)
	{
		if (current->type == TOKEN_WORD)
			result = handle_word_token(&current, &prev_type, result, tokens, shell);
		else if (is_redirection_token(current->type))
			result = handle_redirection_token(&current, &prev_type, result, tokens, shell);
		else
			break;
		if (!result)
		{
			shell->exit_status = 258;
			return (NULL);
		}
	}
	return (result);
}
