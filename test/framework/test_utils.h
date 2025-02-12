#ifndef TEST_UTILS_H
# define TEST_UTILS_H

# include <unistd.h>
# include <stdlib.h>
# include "libft.h"

/**
 * @brief Reads all content from a pipe file descriptor
 * @param fd The file descriptor to read from
 * @return Dynamically allocated string containing the pipe content, or NULL on error
 */
char *read_from_pipe(int fd);

#endif 