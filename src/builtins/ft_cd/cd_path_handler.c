#include "cd.h"
#include "expander/expander.h"

char *handle_path(t_shell *shell, char *arg)
{
	char *expanded;
	char *result;

	expanded = expand_tilde(shell, arg);
	if (!expanded)
		return (NULL);
	if (!ft_strcmp(expanded, ".") || !ft_strcmp(expanded, "..") || \
		expanded[0] == '/' || ft_strncmp(expanded, "./", 2) == 0 || \
		ft_strncmp(expanded, "../", 3) == 0)
		return expanded;
	result = ft_strjoin("./", expanded);
	free(expanded);
	return result;
}