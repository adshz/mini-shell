#ifndef UTILS_H
# define UTILS_H

# include "types.h"
# include "hashtable.h"

// Error handling
int     ft_error(const char *msg, const char *arg);

// Environment utilities
char    *get_env_value(t_env *env, const char *key);
char    **env_to_array(t_env *env);
char    *ft_strjoin3(const char *s1, const char *s2, const char *s3);
void    ft_free_array(char **array);

// Conversion utilities
t_env   *hashmap_to_env(t_hashmap *map);
char    **create_env_array(t_hashmap *env);

// Command utilities
void    free_cmd(void *content);
void    add_command_to_history(t_shell *shell, t_ast_node *cmd);

#endif 