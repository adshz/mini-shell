#include "expander/expander.h"

static char *handle_home_expansion(t_shell *shell)
{
    char *home;

    home = hashmap_get(shell->env, "HOME");
    if (!home)
        return NULL;
    return ft_strdup(home);
}

static char *handle_path_expansion(char *home, const char *arg)
{
    if (arg[1] == '\0')  // Just "~"
        return ft_strdup(home);
    else if (arg[1] == '/')  // "~/something"
        return ft_strjoin(home, arg + 1);
    return ft_strdup(arg);  // Not a tilde expansion case
}

char *expand_tilde(t_shell *shell, const char *arg)
{
    char *home;

    if (!arg || arg[0] != '~')
        return ft_strdup(arg);

    home = handle_home_expansion(shell);
    if (!home)
        return ft_strdup(arg);

    return handle_path_expansion(home, arg);
}
