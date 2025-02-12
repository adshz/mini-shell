#include "parser.h"

t_token *find_last_word_token(t_token *start)
{
	t_token *cmd_token;
	t_token *last_word;
	int is_redir_arg;

	cmd_token = start;
	last_word = NULL;
	is_redir_arg = 0;
	while (cmd_token)
	{
		if (is_redirection_token(cmd_token->type))
		{
			is_redir_arg = 1;
			cmd_token = cmd_token->next;
			continue;
		}
		if (cmd_token->type == TOKEN_WORD)
		{
			if (!is_redir_arg)
				last_word = cmd_token;
			is_redir_arg = 0;
		}
		cmd_token = cmd_token->next;
	}
	return (last_word);
}

t_ast_node *create_default_heredoc_command(void)
{
	t_token *cat_token;
	t_ast_node *node;

	cat_token = create_token(TOKEN_WORD, "cat");
	if (!cat_token)
		return (NULL);
	node = init_command_node(cat_token, 1);
	if (!node)
	{
		free_tokens(cat_token);
		return (NULL);
	}
	if (!fill_args(node->args, cat_token, 1))
	{
		free_tokens(cat_token);
		free_ast(node);
		return (NULL);
	}
	free_tokens(cat_token);
	return (node);
}

t_ast_node *handle_heredoc_command(t_token **tokens, t_shell *shell)
{
	t_token *tmp;
	t_token *next_token;
	t_token *last_word;
	t_ast_node *node;

	tmp = *tokens;
	while (tmp && tmp->next && tmp->next->type == TOKEN_WORD)
		tmp = tmp->next;
	next_token = tmp->next;
	last_word = find_last_word_token(*tokens);
	*tokens = next_token;
	if (last_word)
		node = create_command_from_word(last_word);
	else
		node = create_default_heredoc_command();
	if (!node)
		return (NULL);
	*tokens = tmp;
	return (process_final_redirection(node, tokens, shell));
} 