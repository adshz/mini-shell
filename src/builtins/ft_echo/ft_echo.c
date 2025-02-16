/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:26:08 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:26:39 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "./echo.h"

static bool	is_valid_n_flag(const char *arg)
{
	int	i;

	i = 1;
	if (!arg || !*arg || arg[0] != '-')
		return (false);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (i > 1);
}

static int	skip_and_check_n_flags(char **args, int *has_n_flag)
{
	int	i;

	i = 1;
	*has_n_flag = 0;
	while (args[i] && is_valid_n_flag(args[i]))
	{
		*has_n_flag = 1;
		i++;
	}
	return (i);
}

static void	echo_print_single_argument(const char *arg, int *is_first_arg)
{
	if (!*is_first_arg)
		ft_putchar_fd(' ', STDOUT_FILENO);
	if (arg)
		ft_putstr_fd((char *)arg, STDOUT_FILENO);
	*is_first_arg = 0;
}

static void	echo_print_all_arguments(t_ast_node *node, int start_idx)
{
	int	is_first_arg;

	is_first_arg = 1;
	while (node->args[start_idx])
	{
		echo_print_single_argument(node->args[start_idx], &is_first_arg);
		start_idx++;
	}
}

int	ft_echo(t_shell *shell, t_ast_node *node)
{
	int	start_idx;
	int	has_n_flag;

	if (!node || !node->args)
		return (1);
	start_idx = skip_and_check_n_flags(node->args, &has_n_flag);
	echo_print_all_arguments(node, start_idx);
	if (!has_n_flag)
		ft_putchar_fd('\n', STDOUT_FILENO);
	shell->exit_status = 0;
	return (0);
}
