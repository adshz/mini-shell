#include "expander.h"

int is_quoted(const char *str)
{
    return (str && (str[0] == '\'' || str[0] == '"'));
}

char *handle_quoted_string(const char *str, size_t *i, char quote_char)
{
    size_t start;
    size_t len;
    char *result;

    start = *i;
    (*i)++;  // Skip opening quote
    
    // Find closing quote
    while (str[*i] && str[*i] != quote_char)
        (*i)++;
        
    if (!str[*i])  // No closing quote found
        return (NULL);
        
    len = *i - start - 1;  // -1 to exclude opening quote
    result = ft_substr(str, start + 1, len);
    
    (*i)++;  // Skip closing quote
    return (result);
}

char *strip_quotes(char *str)
{
    char *result;
    size_t i;
    size_t j;
    char quote_char;

    if (!str)
        return (NULL);

    result = malloc(ft_strlen(str) + 1);
    if (!result)
        return (NULL);

    i = 0;
    j = 0;
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            quote_char = str[i++];
            while (str[i] && str[i] != quote_char)
                result[j++] = str[i++];
            if (str[i])
                i++;  // Skip closing quote
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';

    free(str);
    return (result);
} 