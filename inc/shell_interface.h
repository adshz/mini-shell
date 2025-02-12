#ifndef SHELL_INTERFACE_H
# define SHELL_INTERFACE_H

# include "shell.h"
# include <stdbool.h>

/* Shell operation modes */
bool    nonint_shell(t_shell *shell, const char *input);
void    interactive_loop(t_shell *shell);

/* Shell initialization and cleanup */
int     init_shell(t_shell *shell, char *argv[], char *envp[]);
void    cleanup_shell(t_shell *shell);


#endif 