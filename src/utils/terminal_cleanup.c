#include "shell.h"
#include "errors.h"
#include "libft.h"

void cleanup_terminal_state(t_shell *shell)
{
    tcgetattr(STDIN_FILENO, &shell->term_settings);
    if (shell->stdin_backup > 2)
        close(shell->stdin_backup);
    if (shell->stdout_backup > 2)
        close(shell->stdout_backup);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &shell->term_settings) == -1)
        print_error("tcsetattr", "Failed to restore terminal settings", 1);
}

void cleanup_process_state(t_shell *shell)
{
    if (shell->pids)
    {
        free(shell->pids);
        shell->pids = NULL;
    }
    if (shell->old_pwd)
    {
        free(shell->old_pwd);
        shell->old_pwd = NULL;
    }
    if (shell->history)
    {
        char **tmp = shell->history;
        while (*tmp)
        {
            free(*tmp);
            tmp++;
        }
        free(shell->history);
        shell->history = NULL;
    }
}