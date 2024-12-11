#include "minishell.h"

void	echo(char *input)
{
	char	*string_to_print;
	string_to_print = input + 5; // Skip past "echo "
	while (*string_to_print == ' ' || *string_to_print == '\t') // Skip past any spaces or tabs
		string_to_print++;
	if (ft_strncmp(string_to_print, "-n", 2) == 0) // If the user types "echo -n", the program will not add a newline
		printf("%s", string_to_print + 3);
	
	else
		printf("%s\n", string_to_print);
}
