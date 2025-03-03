/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 23:06:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 23:24:21 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

void	skip_spaces_and_get_sign(char *str, int *i, int *sign)
{
	while (str[*i] && str[*i] == ' ')
		(*i)++;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
}

static int	exit_with_num(t_shell *shell, char *str)
{
	int	i;
	int	sign;
	int	exit_status;
	unsigned long long result;

	i = 0;
	sign = 1;
	skip_spaces_and_get_sign(str, &i, &sign);
	if (!ft_isnumber(str + i))
	{
		exit_status = exec_print_err((t_err){ERRNO_EXEC_255, MSGERR_NUMERIC_REQUI, str});
		(cleanup_minishell(shell), exit(exit_status));
	}
	result = 0;
	while (str[i])
	{
		result = (result * 10) + (str[i] - '0');
		if (result > LONG_MAX)
		{
			exit_status = exec_print_err((t_err){ERRNO_EXEC_255, MSGERR_NUMERIC_REQUI, str});
			(cleanup_minishell(shell), exit(exit_status));
		}
		i++;
	}
	return ((result * sign) % 256);
}

void	builtin_exit(t_shell *shell, char **argv)
{
	int	exit_status;

	exit_status = shell->exit_status;
	ft_putendl_fd("exit", STDOUT_FILENO);
	if (argv[1])
	{
		if (argv[2] && ft_isnumber(argv[1]))
		{
			exit_status = exec_print_err(
					(t_err){ERRNO_GENERAL, MSGERR_TOO_MANY_ARGS, NULL});
			tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
			(cleanup_minishell(shell), exit(exit_status));
		}
		else
			exit_status = exit_with_num(shell, argv[1]);
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &shell->original_term);
	(cleanup_minishell(shell), exit(exit_status));
}
