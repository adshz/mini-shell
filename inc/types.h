#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

typedef struct s_ast_node
{
    int type;
    struct s_ast_node *left;
    struct s_ast_node *right;
    // Add other AST node specific fields here
} t_ast_node;

#endif 