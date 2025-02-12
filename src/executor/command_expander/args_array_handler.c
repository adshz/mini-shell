#include "executor/executor.h"
// Handles array operations and updates

static int convert_exit_status_to_str(t_shell *shell, char **expanded)
{
	char exit_str[12];

	ft_itoa_buf(shell->exit_status, exit_str);
	*expanded = ft_strdup(exit_str);
	return (*expanded != NULL);
}

static int process_variable_expansion(t_shell *shell, char *dollar_pos, char **expanded)
{
	ft_putstr_fd("\nDEBUG: process_variable_expansion called with: [", STDERR_FILENO);
	ft_putstr_fd(dollar_pos, STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);

	if (dollar_pos[1] == '?')
	{
		ft_putstr_fd("DEBUG: Converting exit status to string\n", STDERR_FILENO);
		return (convert_exit_status_to_str(shell, expanded));
	}
	
	*expanded = expand_simple_variable(shell, dollar_pos + 1);
	
	ft_putstr_fd("DEBUG: Expanded value: [", STDERR_FILENO);
	if (*expanded)
		ft_putstr_fd(*expanded, STDERR_FILENO);
	else
		ft_putstr_fd("NULL", STDERR_FILENO);
	ft_putstr_fd("]\n", STDERR_FILENO);
	
	return (*expanded != NULL);
}

int handle_expansion(t_shell *shell, char *dollar_pos, char **expanded, bool *was_in_double_quotes)
{
	ft_putstr_fd("\nDEBUG: handle_expansion called\n", STDERR_FILENO);
	ft_putstr_fd("DEBUG: Quote state: ", STDERR_FILENO);
	ft_putstr_fd(shell->in_double_quotes ? "in double quotes" : "not in quotes", STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);

	if (!process_variable_expansion(shell, dollar_pos, expanded))
	{
		ft_putstr_fd("DEBUG: Variable expansion failed\n", STDERR_FILENO);
		shell->in_double_quotes = *was_in_double_quotes;
		return (1);
	}
	ft_putstr_fd("DEBUG: Variable expansion succeeded\n", STDERR_FILENO);
	return (0);
}

static bool should_skip_split_for_export(const char *cmd, const char *arg)
{
	return (ft_strcmp(cmd, "export") == 0 && ft_strchr(arg, '='));
}

static int process_single_argument(t_shell *shell, t_ast_node *node, int *i)
{
	char *space_pos;

	if (process_single_arg_expansion(shell, &node->args[*i]) != 0)
	{
		ft_putstr_fd("DEBUG: Expansion failed for arg[", STDERR_FILENO);
		ft_putnbr_fd(*i, STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		return (SHELL_ERROR);
	}

	if (!shell->in_double_quotes && (space_pos = ft_strchr(node->args[*i], ' ')))
	{
		if (should_skip_split_for_export(node->args[0], node->args[*i]))
		{
			ft_putstr_fd("DEBUG: Skipping split for export variable assignment\n", STDERR_FILENO);
			return (SHELL_SUCCESS);
		}
		if (*(space_pos + 1) == '-')
			return (handle_arg_split(node, i, space_pos));
	}
	return (SHELL_SUCCESS);
}

int process_remaining_args(t_shell *shell, t_ast_node *node)
{
	int i;
	int ret;

	ft_putstr_fd("\nDEBUG: process_remaining_args called\n", STDERR_FILENO);
	i = 1;
	while (node->args[i])
	{
		ft_putstr_fd("DEBUG: Processing arg[", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd("]: [", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);

		ret = process_single_argument(shell, node, &i);
		if (ret != SHELL_SUCCESS)
			return (ret);

		ft_putstr_fd("DEBUG: After expansion arg[", STDERR_FILENO);
		ft_putnbr_fd(i, STDERR_FILENO);
		ft_putstr_fd("]: [", STDERR_FILENO);
		ft_putstr_fd(node->args[i], STDERR_FILENO);
		ft_putstr_fd("]\n", STDERR_FILENO);
		i++;
	}
	return (SHELL_SUCCESS);
}