#ifndef TYPES_H
# define TYPES_H

# include <stdbool.h>

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
} t_env;

#endif 