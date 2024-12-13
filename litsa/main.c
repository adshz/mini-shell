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
		if (ft_strchr(input, '>')) // If the user types '>', the program will redirect the output to a file
			redirect_output(input);
		else
			execute_command(input); // Execute the command
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