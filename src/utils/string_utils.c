#include "utils.h"
#include "libft.h"
#include <stdlib.h>

static int is_in_single_quotes(const char *str, int pos) {
    int quote_count = 0;
    
    for (int i = 0; i < pos; i++) {
        if (str[i] == '\'') {
            quote_count++;
        }
    }  
    return quote_count % 2;
}

void ft_free_array(char **array)
{
    int i;

    if (!array)
        return;
    i = 0;
    while (array[i])
    {
        free(array[i]);
        i++;
    }
    free(array);
}

char *ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
    char *temp;
    char *result;

    if (!s1 || !s2 || !s3)
        return (NULL);
    temp = ft_strjoin(s1, s2);
    if (!temp)
        return (NULL);
    result = ft_strjoin(temp, s3);
    free(temp);
    return (result);
} 

int is_special_char(char c) {
    return (c == '|' || c == '<' || c == '>' || c == ' ' || 
            c == '\t' || c == '\n' || c == ';' || c == '&');
}

char *handle_quotes(const char *input)
{
    int     len;
    char    *result;
    int     j;
    int     i;

    len = ft_strlen(input);
    result = malloc(len + 1);
    j = 0;
    i = 0;
    while (i < len)
    {
        if (input[i] == '\'' && !is_in_single_quotes(input, i))
        {
            i++;
            continue;
        }
        result[j] = input[i];
        j++;
        i++;
    }
    result[j] = '\0';
    return (result);
}