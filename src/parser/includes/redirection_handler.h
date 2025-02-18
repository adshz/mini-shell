#ifndef REDIRECTION_HANDLER_H
#define REDIRECTION_HANDLER_H

t_ast_node	*parse_redirection_construct(t_ast_node *left, t_token **tokens, \
t_shell *shell);
int	is_redirection_token(t_token_type type);
t_ast_node	*create_redirection_node(t_token_type type, \
										const char *file_value);
t_ast_node	*create_default_heredoc_command(void);
#endif
