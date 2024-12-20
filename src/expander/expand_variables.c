#include "expander.h"
#include "libft.h"
#include "shell.h"

char *expand_simple_variable(t_shell *shell, const char *arg)
{
    char *var_value;
    char var_name[1024];
    size_t i;
    size_t j;

    if (!arg)
        return (NULL);

    // Get the variable name
    i = 0;
    j = 0;
    while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
        var_name[j++] = arg[i++];
    var_name[j] = '\0';

    // Get the variable value
    var_value = hashmap_get(shell->env, var_name);
    if (var_value)
    {
        // Remove surrounding quotes if present
        char *clean_value;
        size_t len = ft_strlen(var_value);
        if (len >= 2 && ((var_value[0] == '"' && var_value[len-1] == '"') ||
                        (var_value[0] == '\'' && var_value[len-1] == '\'')))
        {
            clean_value = ft_substr(var_value, 1, len - 2);
        }
        else
        {
            clean_value = ft_strdup(var_value);
        }
        return clean_value;
    }
    return (NULL);
}

char *expand_tilde(t_shell *shell, const char *arg)
{
    if (!arg || arg[0] != '~')
        return ft_strdup(arg);

    char *home = hashmap_get(shell->env, "HOME");
    if (!home)
        return ft_strdup(arg);

    if (arg[1] == '\0')  // Just "~"
        return ft_strdup(home);
    else if (arg[1] == '/')  // "~/something"
        return ft_strjoin(home, arg + 1);
    
    return ft_strdup(arg);  // Not a tilde expansion case
}

char **expand_command(t_shell *shell, const char *cmd)
{
    char *expanded_value;
    char **cmd_args;
    
    // Skip the $ for command expansion
    if (cmd[0] == '$')
    {
        expanded_value = expand_simple_variable(shell, cmd + 1);
        if (!expanded_value)
            return (NULL);
            
        // Split the expanded command into arguments
        cmd_args = ft_split(expanded_value, ' ');
        free(expanded_value);
        
        if (!cmd_args || !cmd_args[0])
        {
            if (cmd_args)
                ft_free_array(cmd_args);
            return (NULL);
        }
        
        return cmd_args;
    }
    
    // Handle non-variable commands
    cmd_args = malloc(2 * sizeof(char *));
    if (!cmd_args)
        return (NULL);
    cmd_args[0] = ft_strdup(cmd);
    cmd_args[1] = NULL;
    
    return cmd_args;
} 