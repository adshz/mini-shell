#include "expander.h"

int is_valid_variable_char(char c)
{
    return (ft_isalnum(c) || c == '_');
}

char *get_variable_name(const char *str, size_t *i)
{
    size_t start;
    size_t len;

    start = *i;
    while (str[*i] && is_valid_variable_char(str[*i]))
        (*i)++;
    
    len = *i - start;
    return (ft_substr(str, start, len));
}

void free_expanded_args(char **args)
{
    size_t i;

    if (!args)
        return;

    i = 0;
    while (args[i])
        free(args[i++]);
    free(args);
} 