// WIFEXITED(status) equivalent: checks if process exited normally
/**
 * @brief Checks if a process exited normally
 *
 * This is a manual implementation of the WIFEXITED macro.
 * It examines the lower 7 bits of the status value to determine
 * if the process terminated normally.
 *
 * @param status The status value returned by wait-family functions
 * @return 1 if the process exited normally, 0 otherwise
 */
int	exited_normally(int status)
{
	return (((status) & 0x7f) == 0);
}

// WTERMSIG(status) equivalent: gets the signal number of the process
/**
 * @brief Gets the signal number that caused a process to terminate
 *
 * This is a manual implementation of the WTERMSIG macro.
 * It extracts the signal number from the status value.
 *
 * @param status The status value returned by wait-family functions
 * @return The signal number that terminated the process
 */
int	get_signal_from_status(int status)
{
	return (status & 0x7f);
}

// WEXITSTATUS(status) equivalent: gets the exit status of the process
/**
 * @brief Gets the exit status of a normally terminated process
 *
 * This is a manual implementation of the WEXITSTATUS macro.
 * It extracts the exit code from bits 8-15 of the status value.
 *
 * @param status The status value returned by wait-family functions
 * @return The exit status of the process (0-255)
 */
int	get_exit_status(int status)
{
	return ((status & 0xff00) >> 8);
}

// WIFSIGNALED(status) equivalent: checks if process was terminated by a signal
/**
 * @brief Checks if a process was terminated by a signal
 *
 * This is a manual implementation of the WIFSIGNALED macro.
 * It examines the lower 7 bits of the status value to determine
 * if the process was terminated by a signal.
 *
 * @param status The status value returned by wait-family functions
 * @return 1 if the process was terminated by a signal, 0 otherwise
 */
int	was_signaled(int status)
{
	return (((status) & 0x7f) != 0);
}