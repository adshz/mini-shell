#include "test_utils.h"

char *read_from_pipe(int fd)
{
    char    buffer[4096];
    char    *output;
    ssize_t bytes_read;
    char    *temp;

    output = NULL;

    while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0)
    {
        buffer[bytes_read] = '\0';
        if (!output)
        {
            output = ft_strdup(buffer);
            if (!output)
                return (NULL);
        }
        else
        {
            temp = ft_strjoin(output, buffer);
            if (!temp)
            {
                free(output);
                return (NULL);
            }
            free(output);
            output = temp;
        }
    }

    if (bytes_read < 0 || !output)
    {
        free(output);
        return (NULL);
    }

    return (output);
} 