/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander_var_expansion.c              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:09:02 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 10:12:20 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"
#include "utils/utils.h"

/* expand command name with variable expand_command_name_with_var
* Phase 2: Variable expansion
* cmd_handle_variable_expansion -> cmd_ex_handle_exit_status
*/
char	*cmd_ex_handle_exit_status(t_shell *shell)
{
	char	exit_str[12];
	char	*expanded;

	ft_itoa_buf(shell->exit_status, exit_str);
	expanded = ft_strdup(exit_str);
	return (expanded);
}

/* expand command name with variable expand_command_name_with_var
* Phase 2: Variable expansion
* cmd_handle_variable_expansion
* - Input: shell context, dollar position
* - Responsibility: Expand variable or $?
* - Output: expanded value
*/
char	*cmd_handle_variable_expansion(t_shell *shell,
		char *dollar_pos, char **var_end)
{
	char	*var_name;
	char	*expanded;

	if (dollar_pos[1] == '?')
		return (cmd_ex_handle_exit_status(shell));
	var_name = cmd_extract_variable_name(dollar_pos, var_end);
	if (!var_name)
		return (NULL);
	expanded = expand_simple_variable(shell, var_name);
	free(var_name);
	return (expanded);
}
