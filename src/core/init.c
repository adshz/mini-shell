/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:43:27 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 22:52:14 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "core.h"
#include "shell.h"
#include "errors.h"
#include "hashtable/hashtable.h"
#include <sys/wait.h>

/**
 * @brief Gets the shell's process ID using fork technique
 *
 * This function determines the shell's PID by using a fork() trick;
 * 1. Creates a child process using fork()
 * 2. Child process exits immediately
 * 3. Parent process waits for child to exit
 * 4. Parent process calculates its PID using child's PID
 *
 * The calculation works because
 *  - fork() returns child's PID to parent
 *  - Child PIDs are typically parent's PID + 1
 *  - Therefore parent's PID = child's PID -1
 *
 * @param shell Pointer to the shell structure to store PID
 *
 * @note Exits program if fork fails;
 */
static void	get_shell_pid(t_shell *shell)
{
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		exit_hanlder(shell, NULL, FORK_ERR, IS_EXIT);
	if (pid == 0)
		exit_handler(shell, NULL, FAILURE, IS_EXIT);
	waitpid(pid, NULL, 0);
	shell->pid = pid - 1;
}

/**
 * @brief Initialises the shell structure and its environement
 *
 * This function performs the complete initialisation of the shell structure:
 *  - Initialise all members of t_shell with zeros
 *  - Sets up environement variables using hash table data structure
 *  - Creates file descriptor backups for I/O restoration
 *  - Confgures terminal settings
 *  - Initialises core environement variables such as PWD, SHIVL, PATH
 *  - Sets up shell process ID
 *
 * The function is called at the beginning of shell (see/main.c)
 *
 * @param shell  Pointer of t_shell type to the shell structure to initailise
 * @param argv   Array of command line arguments
 * @param envp   Array of environment variables (can be NULL)
 * @return SHELL_SUCCESS OR SHELL_ERROR
 *
 * @note If environment initialisation fails, the function exits with ERROR
 * status
 * @note File descriptors are backed up for handling pipes and redirections
 * @note Terminal settings are saved for proper signal handling
 *
 * @see cleanup_shell() for corresponding cleanup
 * @see init_env_vars() for environment variable initialisation
 * @see get_shell_pid() for getting shell process ID
 * 
 */
void	init_shell(t_shell *shell, char *argv[], char *envp[])
{
	ft_memset(shell, 0, sizeof(t_shell));
	shell->environ = envp;
	shell->env = env_to_hashtable(envp);
	shell->stdin = dup(0);
	shell->stdout = dup(1);
	init_env_vars(shell, argv);
	get_shell_pid(shell);
}
