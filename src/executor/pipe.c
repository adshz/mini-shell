#include "shell.h"
#include "executor.h"
#include <unistd.h>

int handle_pipe(t_shell *shell, t_ast_node *node)
{
    int pipefd[2];
    pid_t pid;
    int status;

    if (pipe(pipefd) == -1)
        return (1);

    pid = fork();
    if (pid == -1)
    {
        close(pipefd[0]);
        close(pipefd[1]);
        return (1);
    }

    if (pid == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        exit(execute_command(shell, node->left));
    }
    else
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
        status = execute_command(shell, node->right);
        waitpid(pid, NULL, 0);
        return (status);
    }
} 