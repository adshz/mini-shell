#include "executor/executor.h"

// WTERMSIG(status) equivalent: extracts signal number that terminated the process
int get_signal_from_status(int status)
{
	return (status & 0x7f);
}

// WEXITSTATUS(status) equivalent: gets the exit status of the process
int get_exit_status(int status)
{
	return ((status & 0xff00) >> 8);
}

// WIFSIGNALED(status) equivalent: checks if process was terminated by a signal
int was_signaled(int status)
{
	return (((status) & 0x7f) != 0);
}