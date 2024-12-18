#include "builtins.h"
#include "libft.h"
#include <string.h>
#include <unistd.h>

static const char **get_builtins(void)
{
    static const char *builtins[] = {
        "echo",
        "cd",
        "pwd",
        "export",
        "unset",
        "env",
        "exit",
        NULL
    };
    return builtins;
}

bool is_builtin(const char *cmd)
{
    int i;
    const char **builtins;

    if (!cmd)
        return false;
    i = 0;
    builtins = get_builtins();
    while (builtins[i])
    {
        if (ft_strcmp((char *)cmd, (char *)builtins[i]) == 0)
            return true;
        i++;
    }
    return false;
}

int execute_builtin(t_shell *shell, t_ast_node *node)
{
    if (!node || !node->value)
        return 1;

    if (ft_strcmp(node->value, "echo") == 0)
        return builtin_echo(shell, node);
    if (ft_strcmp(node->value, "cd") == 0)
        return builtin_cd(shell, node);
    if (ft_strcmp(node->value, "pwd") == 0)
        return builtin_pwd(shell, node);
    if (ft_strcmp(node->value, "export") == 0)
        return builtin_export(shell, node);
    if (ft_strcmp(node->value, "unset") == 0)
        return builtin_unset(shell, node);
    if (ft_strcmp(node->value, "env") == 0)
        return builtin_env(shell, node);
    if (ft_strcmp(node->value, "exit") == 0)
        return builtin_exit(shell, node);
    return 1;
}

char *find_command(t_shell *shell, const char *cmd)
{
    char *path;
    char **paths;
    char *full_path;
    int i;

    if (!cmd || !shell)
        return NULL;
    if (cmd[0] == '/' || cmd[0] == '.')
        return ft_strdup(cmd);
    path = hashmap_get(shell->env, "PATH");
    if (!path)
        return NULL;
    paths = ft_split(path, ':');
    if (!paths)
        return NULL;
    i = 0;
    while (paths[i])
    {
        full_path = ft_strjoin3(paths[i], "/", cmd);
        if (full_path && access(full_path, X_OK) == 0)
        {
            ft_free_array(paths);
            return full_path;
        }
        free(full_path);
        i++;
    }
    ft_free_array(paths);
    return NULL;
} 