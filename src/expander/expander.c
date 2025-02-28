/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 15:26:53 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 15:27:41 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "expander/expander.h"

/**
 * @brief Main expander function that expands variables, quotes, and wildcards
 *
 * This function delegates to submodule functions to handle the various
 * expansion steps required for shell command processing.
 *
 * @param shell Shell structure with environment variables
 * @param raw_cmd Raw command string to expand
 * @return char** Array of expanded arguments, NULL on failure
 */
char **expand_raw_command(t_shell *shell, char *raw_cmd)
{
    return (expand_args(shell, raw_cmd));
}
