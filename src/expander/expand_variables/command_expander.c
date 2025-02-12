#include "expander/expander.h"

static char **handle_variable_command(t_shell *shell, const char *cmd)
{
    char *expanded_value;
    char **cmd_args;
    char *prefix = NULL;
    char *dollar_pos;
    char *full_cmd;

    // If command starts with a prefix before $
    if (cmd[0] != '$')
    {
        dollar_pos = ft_strchr(cmd, '$');
        if (dollar_pos)
        {
            size_t prefix_len = dollar_pos - cmd;
            prefix = ft_substr(cmd, 0, prefix_len);
            if (!prefix)
                return NULL;
        }
    }

    // Expand the variable part
    expanded_value = expand_simple_variable(shell, cmd + (prefix ? ft_strlen(prefix) : 0) + 1);
    if (!expanded_value)
    {
        free(prefix);
        return NULL;
    }

    // Combine prefix with expanded value if needed
    if (prefix)
    {
        full_cmd = ft_strjoin(prefix, expanded_value);
        free(prefix);
        free(expanded_value);
        if (!full_cmd)
            return NULL;
        expanded_value = full_cmd;
    }

    // Split into command and args
    cmd_args = ft_split(expanded_value, ' ');
    free(expanded_value);

    if (!cmd_args || !cmd_args[0])
    {
        if (cmd_args)
            ft_free_array(cmd_args);
        return NULL;
    }
    return cmd_args;
}

static char **handle_regular_command(const char *cmd)
{
    char **cmd_args;

    cmd_args = malloc(2 * sizeof(char *));
    if (!cmd_args)
        return NULL;
    cmd_args[0] = ft_strdup(cmd);
    cmd_args[1] = NULL;
    return cmd_args;
}

char **expand_command(t_shell *shell, const char *cmd)
{
    if (cmd[0] == '$')
        return handle_variable_command(shell, cmd);
    return handle_regular_command(cmd);
} 