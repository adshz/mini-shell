#include "executor/executor.h"

static int expand_single_variable(t_shell *shell, char **arg, bool *was_in_double_quotes)
{
    char *prefix;
    char *suffix;
    char *expanded;
    char *result;

    ft_putstr_fd("\nDEBUG: expand_single_variable called with arg: [", STDERR_FILENO);
    ft_putstr_fd(*arg, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    prefix = get_prefix(*arg, ft_strchr(*arg, '$'));
    suffix = get_suffix(ft_strchr(*arg, '$'));

    ft_putstr_fd("DEBUG: Prefix: [", STDERR_FILENO);
    if (prefix)
        ft_putstr_fd(prefix, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    ft_putstr_fd("DEBUG: Suffix: [", STDERR_FILENO);
    if (suffix)
        ft_putstr_fd(suffix, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    if (handle_expansion(shell, ft_strchr(*arg, '$'), &expanded, was_in_double_quotes))
    {
        ft_putstr_fd("DEBUG: Expansion failed\n", STDERR_FILENO);
        free(prefix);
        free(suffix);
        return (SHELL_ERROR);
    }

    ft_putstr_fd("DEBUG: Expanded value: [", STDERR_FILENO);
    if (expanded)
        ft_putstr_fd(expanded, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    result = combine_parts(prefix, expanded, suffix);
    ft_putstr_fd("DEBUG: Final combined result: [", STDERR_FILENO);
    if (result)
        ft_putstr_fd(result, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    return (replace_argument(arg, result));
}

static int process_remaining_variables(t_shell *shell, char **result)
{
    char *temp_suffix;

    temp_suffix = *result;
    if (ft_strchr(temp_suffix, '$'))
    {
        if (process_single_arg_expansion(shell, &temp_suffix))
        {
            free(*result);
            return (SHELL_ERROR);
        }
        *result = temp_suffix;
    }
    return (0);
}

int process_single_arg_expansion(t_shell *shell, char **arg)
{
    bool was_in_double_quotes;
    int ret;

    ft_putstr_fd("\nDEBUG: process_single_arg_expansion called with: [", STDERR_FILENO);
    ft_putstr_fd(*arg, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    init_quote_state(shell, *arg, &was_in_double_quotes);
    ft_putstr_fd("DEBUG: Initial quote state: ", STDERR_FILENO);
    ft_putstr_fd(shell->in_double_quotes ? "in double quotes" : "not in quotes", STDERR_FILENO);
    ft_putstr_fd("\n", STDERR_FILENO);

    if (!ft_strchr(*arg, '$'))
    {
        ft_putstr_fd("DEBUG: No $ found in argument\n", STDERR_FILENO);
        shell->in_double_quotes = was_in_double_quotes;
        return (0);
    }

    ret = expand_single_variable(shell, arg, &was_in_double_quotes);
    if (ret != 0)
    {
        ft_putstr_fd("DEBUG: expand_single_variable failed\n", STDERR_FILENO);
        return (ret);
    }

    ret = process_remaining_variables(shell, arg);
    ft_putstr_fd("DEBUG: Final expanded argument: [", STDERR_FILENO);
    ft_putstr_fd(*arg, STDERR_FILENO);
    ft_putstr_fd("]\n", STDERR_FILENO);

    shell->in_double_quotes = was_in_double_quotes;
    return (ret);
}