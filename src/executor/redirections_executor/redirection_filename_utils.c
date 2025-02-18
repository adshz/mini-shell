/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_filename_utils.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 15:23:34 by szhong            #+#    #+#             */
/*   Updated: 2025/01/29 15:25:04 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../executor.h"
#include "utils/utils.h"

char	*get_expanded_filename(t_shell *shell, const char *filename)
{
	char	*expanded;

	expanded = expand_redirection_filename_variable(shell, filename);
	if (expanded)
		return (expanded);
	expanded = handle_quoted_filename(shell, filename);
	if (expanded)
		return (expanded);
	return (ft_strdup(filename));
}

int	is_ambiguous_redirect(t_shell *shell, const char *value)
{
	char	*expanded;

	expanded = NULL;
	if (!ft_strchr(value, '$'))
		return (0);
	expanded = expand_variable(shell, value);
	if (!expanded)
		return (1);
	if (ft_strchr(expanded, ' '))
	{
		free(expanded);
		return (1);
	}
	free(expanded);
	return (0);
}

int	handle_ambiguous_filename(const char *filename)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd((char *)filename, STDERR_FILENO);
	ft_putstr_fd(": ambiguous redirect\n", STDERR_FILENO);
	return (1);
}
