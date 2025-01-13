#ifndef EXPANDER_H
# define EXPANDER_H

# include "shell.h"
# include "libft.h"

/* Variable expansion */
char    *expand_simple_variable(t_shell *shell, const char *arg);
char    *expand_complex_variable(t_shell *shell, const char *arg);
char    **expand_command(t_shell *shell, const char *cmd);
char    *expand_word(t_shell *shell, const char *word);

/* Quote handling */
char    *strip_quotes(char *str);
int     is_quoted(const char *str);
char    *handle_quoted_string(const char *str, size_t *i, char quote_char);

/* Utility functions */
int     is_valid_variable_char(char c);
char    *get_variable_name(const char *str, size_t *i);
void    free_expanded_args(char **args);

/* Special Expander functions  */
char    *expand_exit_status(t_shell *shell);
char    *expand_home_dir(t_shell *shell, const char *arg);
char    *expand_tilde(t_shell *shell, const char *arg);

#endif 