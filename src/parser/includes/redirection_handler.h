#ifndef REDIRECTION_HANDLER_H
#define REDIRECTION_HANDLER_H

t_ast_node	*parse_redirection_construct(t_ast_node *left, t_token **tokens, \
t_shell *shell);
int	is_redirection_token(t_token_type type);
t_ast_node	*create_redirection_node(t_token_type type, \
										const char *file_value);
t_ast_node	*create_default_heredoc_command(void);


void init_word_token(t_word_token *wt, t_redir_parse *parse, t_token **tokens);
void update_token_positions(t_word_token *wt);
t_ast_node *create_and_attach_command(t_word_token *wt);
t_ast_node *is_word_part_of_redirection(t_word_token *wt);
t_ast_node *handle_word_token(t_word_token *wt);


t_ast_node	*process_redir_token(t_redir_parse *parse, t_token **tokens);

t_ast_node	*handle_heredoc_token(t_redir_token *rt);
#endif