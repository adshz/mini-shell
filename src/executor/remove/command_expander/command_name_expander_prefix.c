/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander_prefix.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 23:38:32 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 23:39:17 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "executor/executor.h"

/* expand command name with variable expand_command_name_with_var
* Phase 1: Prefix Extraction
* cmd_handle_prefix_extraction
* - Input: command string, dollar position
* - Responsibility: Extract prefix before variable
* - Output: prefix string or NULL
*/
int	cmd_handle_prefix_extraction(const char *cmd,
		char *dollar_pos, char **prefix)
{
	size_t	prefix_len;

	prefix_len = dollar_pos - cmd;
	if (prefix_len > 0)
	{
		*prefix = ft_substr(cmd, 0, prefix_len);
		if (!*prefix)
			return (1);
	}
	else
		*prefix = NULL;
	return (0);
}

/* expand command name with variable expand_command_name_with_var
* Phase 2: Variable Name Extraction
* cmd_extract_variable_name
* - Input: dollar position, end position pointer
* - Responsibility: Extract variable name
* - Output: variable name string
*/
char	*cmd_extract_variable_name(char *dollar_pos, char **var_end)
{
	char	*var_name;
	char	*var_start;
	size_t	var_len;

	var_start = dollar_pos + 1;
	*var_end = var_start;
	while (**var_end && (ft_isalnum(**var_end) || **var_end == '_'))
		(*var_end)++;
	var_len = *var_end - var_start;
	var_name = ft_substr(var_start, 0, var_len);
	return (var_name);
}
