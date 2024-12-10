#include "minishell.h"

int	main(int argc, char **argv) 
{
  char *input; // to store the user input

  (void) argc;
  (void) argv;
  while (1) // Infinite loop to keep the shell running
  {
	input = readline("minishell->$ "); // Prompt the user for input
	if (!input) // If the user presses Ctrl+D, the program will exit
	  break;
	if (input[0] != '\0') // If the user presses Enter without typing anything, the program will not add an empty line to the history
	{
		add_history(input); // Add the input to the history
		if (ft_strncmp(input, "echo", 4) == 0) // If the user types "echo", the program will print the input
	  		echo(input);
		else if (ft_strncmp(input, "pwd", 3) == 0) // If the user types "pwd", the program will print the current working directory
			pwd();
	}
	if (ft_strncmp(input, "exit", 4) == 0) // If the user types "exit", the program will exit
	{
	  free(input);
	  break;
	}
	free(input);
  }
  return 0;
}