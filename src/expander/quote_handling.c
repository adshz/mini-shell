#include "expander.h"

int is_quoted(const char *str)
{
    return (str && (str[0] == '\'' || str[0] == '"'));
}

static void handle_string_copy(char *result, const char *str,
        size_t *i, size_t *j, char quote_char)
{
        if (quote_char)
        {
                (*i)++;  // Skip opening quote
                while (str[*i] && str[*i] != quote_char)
                {
                        result[*j] = str[*i];
                        (*j)++;
                        (*i)++;
                }
                if (str[*i])
                        (*i)++;  // Skip closing quote
        }
        else
        {
                result[*j] = str[*i];
                (*j)++;
                (*i)++;
        }
}

static char *process_string(const char *str)
{
        char    *result;
        size_t  i;
        size_t  j;

        result = malloc(ft_strlen(str) + 1);
        if (!result)
                return (NULL);
        i = 0;
        j = 0;
        while (str[i])
        {
                if (str[i] == '\'' || str[i] == '"')
                    handle_string_copy(result, str, &i, &j, str[i]);
                else
                    handle_string_copy(result, str, &i, &j, 0);
        }
        result[j] = '\0';
        return (result);
}

char *strip_quotes(char *str)
{
        char *result;

        if (!str)
                return (NULL);
        result = process_string(str);
        free(str);
        return (result);
} 