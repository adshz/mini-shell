#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <termios.h>
# include <sys/types.h>

// Forward declarations for types used in t_shell
struct s_token;
struct s_ast_node;
struct s_hashmap;
struct s_list;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_minishell
{
    char            *line;
    struct s_token  *tokens;
    struct s_ast_node *ast;
    struct s_hashmap *env;
    struct s_list   *cmds;
    pid_t           pid;
    pid_t           *pids;
    char            *old_pwd;
    char            **history;
    int             exit_status;
    int             stdin_backup;
    int             stdout_backup;
    struct termios  term_settings;
    bool            heredoc_sigint;
    bool            signint_child;
} t_shell;

#endif 