#include "minishell.h"

void	echo(char *input)
{
	char	*string_to_print;
	string_to_print = input + 5; // Skip past "echo "
	printf("%s\n", string_to_print); 
}
