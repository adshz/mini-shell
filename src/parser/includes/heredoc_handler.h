#ifndef HEREDOC_HANDLER_H
#define HEREDOC_HANDLER_H

t_ast_node       *handle_heredoc_command(t_token **tokens, t_shell *shell);
t_ast_node   *process_initial_redirection(t_token **tokens, t_shell *shell);
t_ast_node   *process_final_redirection(t_ast_node *node, \
										t_token **tokens, \
										t_shell *shell);
#endif
