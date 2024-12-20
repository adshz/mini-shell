#include "expander.h"

char *expand_variables(t_shell *shell, const char *arg)
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
        return (ft_strdup(var_value));

    return (NULL);
}

char **expand_command(t_shell *shell, const char *cmd)
{
    char *expanded_value;
    char **cmd_args;
    
    // Skip the $ for command expansion
    if (cmd[0] == '$')
        expanded_value = expand_variables(shell, cmd + 1);
    else
        expanded_value = ft_strdup(cmd);
        
    if (!expanded_value)
        return (NULL);
        
    // Split the expanded command into arguments
    cmd_args = ft_split(expanded_value, ' ');
    free(expanded_value);
    
    return (cmd_args);
} 