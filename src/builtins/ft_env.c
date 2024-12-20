/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 20:30:39 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 20:30:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "builtins.h"
#include "executor.h"

static void print_env_var(const char *key, const char *value)
{
    char *key_copy = ft_strdup(key);
    char *value_copy = ft_strdup(value);
    
    ft_putstr_fd(key_copy, STDOUT_FILENO);
    ft_putstr_fd("=", STDOUT_FILENO);
    ft_putendl_fd(value_copy, STDOUT_FILENO);
    
    free(key_copy);
    free(value_copy);
}

int builtin_env(t_shell *shell, t_ast_node *node)
{
    char *cmd_path;
    char **env_array;
    pid_t pid;
    int status;

    if (!node->args[1])
    {
        hashmap_iterate(shell->env, print_env_var);
        return (0);
    }

    // Try to execute the command
    cmd_path = get_command_path(node->args[1], shell->env);
    if (!cmd_path)
    {
        ft_putstr_fd("env: ", STDERR_FILENO);
        ft_putstr_fd(node->args[1], STDERR_FILENO);
        ft_putendl_fd(": No such file or directory", STDERR_FILENO);
        return (127);
    }

    env_array = create_env_array(shell->env);
    if (!env_array)
    {
        free(cmd_path);
        return (1);
    }

    pid = fork();
    if (pid == 0)
    {
        execve(cmd_path, node->args + 1, env_array);
        exit(127);
    }
    else if (pid > 0)
    {
        waitpid(pid, &status, 0);
        free(cmd_path);
        ft_free_array(env_array);
        return (WEXITSTATUS(status));
    }

    free(cmd_path);
    ft_free_array(env_array);
    return (1);
}


