#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>
# include <termios.h>
# include <sys/types.h>

// Forward declarations for types used in t_shell
struct s_token;
struct s_hashmap;
struct s_list;

typedef enum e_ast_type
{
    AST_COMMAND,     // Simple command
    AST_PIPE,        // Pipeline
    AST_REDIR_IN,    // Input redirection (<)
    AST_REDIR_OUT,   // Output redirection (>)
    AST_REDIR_APPEND,// Append redirection (>>)
    AST_HEREDOC,     // Here document (<<)
    AST_AND,         // AND operator (&&)
    AST_OR,          // OR operator (||)
    AST_VAR_EXPANSION // Variable expansion ($var)
} t_ast_type;

typedef struct s_ast_node
{
    t_ast_type          type;
    char                *value;      // For commands and file names
    char                **args;      // For command arguments
    int                 is_expanded; // Flag to indicate if this node contains expanded variables
    char                *original;   // Original unexpanded value (for variable expansions)
    struct s_ast_node   *left;
    struct s_ast_node   *right;
} t_ast_node;

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_minishell
{
    char            **argv;
    char            **envp;
    char            *line;
    struct s_token  *tokens;
    t_ast_node      *ast;
    struct s_hashmap *env;
    struct s_hashmap *local_vars;
    struct s_hashmap *alias;
    struct s_list   *cmds;
    pid_t           pid;
    pid_t           *pids;
    int             pid_count;
    char            *old_pwd;
    char            **history;
    int             history_size;
    int             history_capacity;
    int             exit_status;
    int             stdin_backup;
    int             stdout_backup;
    struct termios  term_settings;
    bool            heredoc_sigint;
    bool            signint_child;
    bool            in_pipe;
    bool            in_heredoc;
} t_shell;

#endif 