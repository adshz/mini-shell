#ifndef REDIRECTIONS_EXECUTOR_H
#define REDIRECTIONS_EXECUTOR_H

void       setup_redirections(t_shell *shell, t_ast_node *node);
int        execute_redirection(t_shell *shell, t_ast_node *node);

#endif
