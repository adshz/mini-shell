#include "minishell.h"

void	redirect_output(char *input)
{
	int	fd;
	char	*file_name;
	int	stdout_original;

	stdout_original = dup(STDOUT_FILENO); // Save the standard output file descriptor
	file_name = ft_strchr(input, '>'); // Find the first occurrence of '>'
	if (!file_name)
		return ;
	*file_name++ = '\0'; // Null-terminate the command at the position of '>' and move the pointer to the next character
	while (*file_name == ' ') // Skip spaces
		file_name++;
// Open the file in write-only mode, create it if it does not exist, if it exists lear its contents
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644); // 0644 is the file permissions (rw-r--r--)
	if (fd == -1)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return ;
	}
// Redirect standard output to the file
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);
	execute_command(input); // Execute the command
	dup2(stdout_original, STDOUT_FILENO); // Restore the standard output
	close(stdout_original);
}

void	redirect_input(char *input)
{
	char	*file_name;
	int	fd;
	int	stdin_original;

	stdin_original = dup(STDIN_FILENO); // Save the standard input file descriptor
	file_name = ft_strchr(input, '<'); // Find the first occurrence of '<'
	if (!file_name)
		return ;
	*file_name++ = '\0'; // Null-terminate the command at the position of '<' and move the pointer to the next character
	while (*file_name == ' ') // Skip spaces
		file_name++;
// Open the file in read-only mode
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
	{
		printf("minishell: syntax error near unexpected token `newline'\n");
		return ;
	}
	// Redirect standard input to the file
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return ;
	}
	close(fd);

	execute_command(input); // Execute the command

	dup2(stdin_original, STDIN_FILENO); // Restore the standard input
	close(stdin_original);
}