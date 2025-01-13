# ifndef REDIRECTIONS_H
# define REDIRECTIONS_H

# include "shell.h"
# include <stdbool.h>

// Helper functions
bool is_ambiguous_redirect(t_shell *shell, const char *filename);

// Redirection handlers
int handle_input_redirection(t_shell *shell, const char *filename);
int handle_output_redirection(t_shell *shell, const char *filename);
int handle_append_redirection(t_shell *shell, const char *filename);

#endif 