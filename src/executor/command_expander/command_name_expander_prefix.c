/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_name_expander_prefix.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/13 10:09:02 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/13 10:12:20 by evmouka          ###   ########.fr       */
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

	ft_printf("DEBUG [cmd_prefix]: Extracting prefix from command: [%s]\n", cmd);
	
	prefix_len = dollar_pos - cmd;
	if (prefix_len > 0)
	{
		*prefix = ft_substr(cmd, 0, prefix_len);
		if (!*prefix)
			return (1);
		ft_printf("DEBUG [cmd_prefix]: Extracted prefix: [%s]\n", *prefix);
	}
	else
	{
		*prefix = NULL;
		ft_printf("DEBUG [cmd_prefix]: No prefix found\n");
	}
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

	ft_printf("DEBUG [cmd_var]: Extracting variable name\n");
	var_start = dollar_pos + 1;  // Skip the $
	*var_end = var_start;
	
	// Find end of variable name
	while (**var_end && (ft_isalnum(**var_end) || **var_end == '_'))
		(*var_end)++;
	
	var_len = *var_end - var_start;
	var_name = ft_substr(var_start, 0, var_len);
	
	if (var_name)
		ft_printf("DEBUG [cmd_var]: Extracted variable name: [%s]\n", var_name);
	else
		ft_printf("DEBUG [cmd_var]: Failed to extract variable name\n");
	
	return (var_name);
} 