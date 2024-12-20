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
    {
        ft_putstr_fd("Debug: is_builtin - cmd is NULL\n", 2);
        return false;
    }
    
    ft_putstr_fd("Debug: is_builtin - Checking command: '", 2);
    ft_putstr_fd((char *)cmd, 2);
    ft_putstr_fd("'\n", 2);
    
    i = 0;
    builtins = get_builtins();
    ft_putstr_fd("Debug: is_builtin - Available builtins:\n", 2);
    while (builtins[i])
    {
        ft_putstr_fd("  '", 2);
        ft_putstr_fd((char *)builtins[i], 2);
        ft_putstr_fd("'\n", 2);
        if (ft_strcmp((char *)cmd, (char *)builtins[i]) == 0)
        {
            ft_putstr_fd("Debug: is_builtin - Found matching builtin\n", 2);
            return true;
        }
        i++;
    }
    ft_putstr_fd("Debug: is_builtin - No matching builtin found\n", 2);
    return false;
}

int execute_builtin(t_shell *shell, t_ast_node *node)
{
    if (!node || !node->value)
    {
        ft_putstr_fd("Debug: node or node->value is NULL\n", 2);
        return 1;
    }

    ft_putstr_fd("Debug: Executing builtin command: ", 2);
    ft_putstr_fd(node->value, 2);
    ft_putchar_fd('\n', 2);

    // Debug print args
    int i = 0;
    ft_putstr_fd("Debug: Command arguments:\n", 2);
    while (node->args && node->args[i])
    {
        ft_putstr_fd("arg[", 2);
        ft_putnbr_fd(i, 2);
        ft_putstr_fd("]: '", 2);
        ft_putstr_fd(node->args[i], 2);
        ft_putstr_fd("'\n", 2);
        i++;
    }

    if (ft_strcmp(node->value, "echo") == 0)
    {
        ft_putstr_fd("Debug: Calling ft_echo\n", 2);
        return ft_echo(shell, node);
    }
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

    ft_putstr_fd("Debug: Command not found as builtin\n", 2);
    return 1;
}

char *find_command(t_shell *shell, const char *cmd)
{
	char	*path;
	char	**paths;
	char	*full_path;
	int		i;

	if (!cmd || !shell)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	path = hashmap_get(shell->env, "PATH");
	if (!path)
	{
		handle_error(shell, ERR_FILE_NOT_FOUND, "PATH");
		return (NULL);
	}
	paths = ft_split(path, ':');
	if (!paths)
	{
		handle_error(shell, ERR_MALLOC, NULL);
		return (NULL);
	}
	i = 0;
	while (paths[i])
	{
		full_path = ft_strjoin3(paths[i], "/", cmd);
		if (full_path && access(full_path, X_OK) == 0)
		{
			ft_free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free_array(paths);
	handle_error(shell, ERR_CMD_NOT_FOUND, cmd);
	return (NULL);
} 