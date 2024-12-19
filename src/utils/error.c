#include "errors.h"
#include <stdio.h>

int print_error(char *cmd, char *msg, int error_code)
{
    if (cmd)
        fprintf(stderr, "minishell: %s: %s\n", cmd, msg);
    else
        fprintf(stderr, "minishell: %s\n", msg);
    return (error_code);
} 