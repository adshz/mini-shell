#include "builtin_utils.h"

const char *skip_quotes(const char *str)
{
    const char *start;

    start = str;
    if (*start == '"' && str[ft_strlen(str) - 1] == '"')
        start++;
    else if (*start == '\'' && str[ft_strlen(str) - 1] == '\'')
        start++;
    return (start);
}

bool check_first_char(const char *start)
{
    if (!*start || *start == '"' || *start == '\'')
        return (false);
    if (!ft_isalpha(*start) && *start != '_' && *start != '$')
        return (false);
    return (true);
}

bool is_valid_identifier(const char *str)
{
    int i;
    const char *start;

    if (!str || !*str)
        return (false);
    start = skip_quotes(str);
    if (!check_first_char(start))
        return (false);
    i = 1;
    while (start[i] && start[i] != '=')
    {
        if (!ft_isalnum(start[i]) && start[i] != '_')
        {
            return (false);
        }
        i++;
    }
    return (true);
} 