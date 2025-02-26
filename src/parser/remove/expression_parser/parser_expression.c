/* ************************************************************************** */
/*																			  */
/*														  :::	   ::::::::   */
/*	 parser_expression.c								:+:		 :+:	:+:   */
/*													  +:+ +:+		  +:+	  */
/*	 By: szhong <szhong@student.42london.com>		+#+  +:+	   +#+		  */
/*												  +#+#+#+#+#+	+#+			  */
/*	 Created: 2025/02/19 18:19:23 by szhong			   #+#	  #+#			  */
/*	 Updated: 2025/02/19 18:19:23 by szhong			  ###	########.fr		  */
/*																			  */
/* ************************************************************************** */
#include "parser/parser.h"

bool	is_binary_operator(t_shell *shell)
{
	t_token_type	type;

	if (!shell->curr_token)
		return (false);
	type = shell->curr_token->type;
	if (type == TOKEN_PIPE || type == TOKEN_AND || type == TOKEN_OR)
		return (true);
	return (false);
}

t_ast_node	*ft_create_new_node(t_ast_node_type type)
{
	t_ast_node	*new_node;

	new_node = (t_ast_node *)ft_calloc(1, sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	return (new_node);
}

bool	ft_is_redirection(t_token_type type)
{
	if (type == TOKEN_LESS || type == TOKEN_GREAT || type == TOKEN_DLESS \
		|| type == TOKEN_DGREAT)
		return (true);
	return (false);
}

char	*ft_strjoin3(const char *s1, const char *s2, char c)
{
	char	*joined;
	size_t	total_len;
	size_t	i;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	if (!c || !ft_strlen(s1) || !ft_strlen(s2))
		return (ft_strjoin(s1, s2));
	total_len = ft_strlen(s1) + ft_strlen(s2) + 1 + 1;
	joined = ft_calloc(total_len, sizeof(char));
	if (!joined)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		joined[i] = s1[i];
		i++;
	}
	joined[i++] = c;
	j = 0;
	while (s2[j])
		joined[i++] = s2[j++];
	joined[i] = '\0';
	return (joined);
}

bool	ft_build_raw_command_string(t_shell *shell, char **raw_cmd_ptr)
{
	char	*to_free;

	if (shell->parse_err.type)
		return (false);
	if (!*raw_cmd_ptr)
		*raw_cmd_ptr = ft_strdup("");
	if (!*raw_cmd_ptr)
		return (false);
	while (shell->curr_token && shell->curr_token->type == TOKEN_IDENTIFIER)
	{
		to_free = *raw_cmd_ptr;
		*raw_cmd_ptr = ft_strjoin3(*raw_cmd_ptr, shell->curr_token->value, ' ');
		if (!*raw_cmd_ptr)
			return (free(to_free), false);
		free(to_free);
		ft_get_next_token(shell);
	}
	return (true);
}

void	ft_clear_io_list(t_io_node	**lst)
{
	t_io_node	*curr_node;
	t_io_node	*next;

	curr_node = *lst;
	if (!curr_node)
		return ;
	while (curr_node)
	{
		free(curr_node->value);
		free_char2(curr_node->expanded_value);
		next = curr_node->next;
		free(curr_node);
		curr_node = next;
	}
	*lst = NULL;
}

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

void	clean_cmd_node(t_ast_node *node)
{
	if (!node)
		return ;
	ft_clear_io_list(&(node->io_list));
	free(node->raw_command);
	free_char2(node->expanded_argv);
}

t_io_type	convert_to_io_type(t_token_type type)
{
	if (type == TOKEN_LESS)
		return (IO_IN);
	if (type == TOKEN_GREAT)
		return (IO_OUT);
	if (type == TOKEN_DLESS)
		return (IO_HEREDOC);
	return (IO_APPEND);
}

t_ast_node_type	ft_get_ast_node_type(t_token_type type)
{
	if (type == TOKEN_AND)
		return (NODE_AND);
	if (type == TOKEN_OR)
		return (NODE_OR);
	return (NODE_PIPE);
}

t_io_node	*ft_create_io_node(t_token_type type, char *value)
{
	t_io_node	*new_io_node;

	new_io_node = (t_node *)ft_calloc(1, sizeof(t_io_node));
	if (!new_io_node)
		return (NULL);
	new_io_node->type = convert_to_io_type(type);
	new_io_node->value = ft_strdup(value);
	if (!new_io_node->value)
		return (free(new_io_node), NULL);
	return (new_io_node);
}

void	append_io_node(t_io_node **lst, t_io_node *new)
{
	t_io_node	*curr_node;

	if (!*lst)
	{
		*lst = new;
		return ;
	}
	curr_node = *lst;
	while (curr_node && curr_node->next)
		curr_node = curr_node->next;
	curr_node->next = new;
}

bool	ft_process_redirection(t_shell *shell, t_io_node **io_lst)
{
	t_token_type	redir_token_type;
	t_io_node		*tmp_io_node;

	if (shell->parse_err.type)
		return (false);
	while (shell->curr_token && ft_is_redirection(shell->curr_token->type))
	{
		redir_token_type = shell->curr_token->type;
		ft_get_next_token(shell);
		if (!shell->curr_token)
			return (set_parse_err(E_SYNTAX), false);
		if (shell->curr_token->type != TOKEN_IDENTIFIER)
			return (set_parse_err(E_SYNTAX), false);
		tmp_io_node = ft_create_io_node(redir_token_type, \
								shell->curr_token->value);
		if (!tmp_io_node)
			return (set_parse_err(E_MEM), false);
		append_io_node(io_list, tmp_io_node);
		ft_get_next_token(shell);
	}
	return (true);
}

t_ast_node	*ft_build_command_subtree(t_shell *shell)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	node = ft_create_new_node(NODE_CMD);
	if (!node)
		return (set_parse_err(E_MEM), NULL);
	while (shell->curr_token && (shell->curr_token->type == TOKEN_IDENTIFIER) \
		|| ft_is_redirection(shell->curr_token->type))
	{
		if (shell->curr_token->type == TOKEN_IDENTIFIER)
		{
			if (!ft_build_raw_command_string(shell, &(node->raw_command)))
				return (clean_cmd_node(node), set_parse_err(E_MEM), NULL);
		}
		else if (ft_is_redirection(shell->curr_token->type))
		{
		if (!ft_process_redirection(shell, &(node->io_list)))
				return (free(node->raw_command), free(node), NULL);
		}
	}
	return (node);
}

t_ast_node	*ft_parse_atomic_expression(t_shell *shell)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	if (is_binary_operator(shell) || shell->curr_token->type == TOKEN_C_PARENT)
		return (set_parse_err(shell, E_SYNTAX), NULL);
	else if (shell->curr_token->type == TOKEN_O_PARENT)
	{
		ft_get_next_token(shell);
		node = ft_parse_expression(shell, 0);
		if (!node)
			return (set_parse_err(E_MEM), NULL);
		if (!shell->curr_token || shell->curr_token->type != TOKEN_C_PARENT)
			return (set_parse_err(E_SYNTAX), node);
		ft_get_next_token(shell);
		return (node);
	}
	else
		return (ft_build_command_subtree(shell));
}

int	ft_precedence(t_token_type type)
{
	if (type == TOKEN_OR || type == TOKEN_AND)
		return (0);
	return (1);
}

int	curr_token_precd(t_shell *shell)
{
	return (ft_precedence(shell->curr_token->type));
}

t_ast_node	*ft_combine_node(t_shell *shell, t_token_type operator, \
							t_ast_node *left, t_ast_node *right)
{
	t_ast_node	*node;

	if (shell->parse_err.type)
		return (NULL);
	node = ft_create_new_node(ft_get_ast_node_type(operator)));
	if (!node)
		return (set_parse_err(E_MEM), NULL);
	node->left = left;
	node->right = right;
	return (node);
}

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

// please check if you have put them in lexer?
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

void	ft_cleanup_ast(t_shell *shell, t_ast_node **ast)
{
	recursively_ft_cleanup_ast(*ast);
	*ast = NULL;
	clean_token_lst(&shell->tokens);
}

t_ast_node	*ft_parse_expression(t_shell *shell, int min_prec)
{
	t_ast_node		*left;
	t_ast_node		*right;
	int				n_prec;
	t_token_type	operator;

	if (shell->parse_err.type ||!shell->curr_token)
		return (NULL);
	left = ft_parse_atomic_expression(shell);
	if (!left)
		return (NULL);
	while (is_binary_operator(shell) && curr_token_precd(shell) >= mini_prec)
	{
		operator = shell->curr_token->type;
		ft_get_next_token(shell);
		if (!shell->curr_token)
			return (set_parse_err(E_SYNTAX), left);
		n_prec = ft_precedence(operator) + 1;
		right = ft_parse_expression(shell, n_prec);
		if (!right)
			return (left);
		left = ft_combine_node(operator, left, right);
		if (!left)
			return (ft_cleanup_ast(&left), ft_cleanup_ast(&right));
	}
	return (left);
}
