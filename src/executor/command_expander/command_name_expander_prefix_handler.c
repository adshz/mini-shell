/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander_prefix_handler.c             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:40:28 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:40:31 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

/* expand command name with variable expand_command_name_with_var
* Phase 1: Prefix handling
* cmd_handle_prefix_extraction -> cmd_create_command_prefix
*/
int	cmd_create_command_prefix(const char *arg, size_t prefix_len, char **prefix)
{
	*prefix = ft_substr(arg, 0, prefix_len);
	return (*prefix != NULL);
}

/* expand command name with variable expand_command_name_with_var
* Phase 1: Prefix handling
* cmd_handle_prefix_extraction
* - Input: original command, dollar position
* - Responsibility: Extract any text before the variable
* - Output: prefix string or NULL
*/
int	cmd_handle_prefix_extraction(const char *arg, char *dollar_pos, \
								char **prefix)
{
	size_t	prefix_len;

	*prefix = NULL;
	if (dollar_pos <= arg)
		return (0);
	prefix_len = dollar_pos - arg;
	if (!cmd_create_command_prefix(arg, prefix_len, prefix))
		return (1);
	return (0);
}

/* expand command name with variable expand_command_name_with_var
* Phase 1: Prefix handling
* cmd_handle_prefix_extraction -> cmd_create_command_prefix
*/
char	*cmd_extract_variable_name(char *dollar_pos, char **var_end)
{
	char	*var_name;
	size_t	var_len;

	*var_end = dollar_pos + 1;
	while (**var_end && (ft_isalnum(**var_end) || **var_end == '_'))
		(*var_end)++;
	var_len = *var_end - (dollar_pos + 1);
	var_name = ft_substr(dollar_pos + 1, 0, var_len);
	return (var_name);
}
