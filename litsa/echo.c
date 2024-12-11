#include "minishell.h"

void	echo(char *input)
{
	char	*string_to_print;
	int		n_flag;

	n_flag = 0;
	string_to_print = input + 4; // Skip past "echo "
	while (*string_to_print == ' ' || *string_to_print == '\t') // Skip past any spaces or tabs
		string_to_print++;
	if (ft_strncmp(string_to_print, "-n", 2) == 0) // If the user types "echo -n", the program will not add a newline
	{
		n_flag = 1;
		string_to_print += 2; // Skip past "-n"
		while (*string_to_print == ' ' || *string_to_print == '\t') // Skip past any spaces or tabs	
			string_to_print++;
		printf("%s", string_to_print); // Print the input
	}
	if (!n_flag)
		printf("%s\n", string_to_print); // Print the input
}
