/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_array_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:20 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "executor/executor.h"

/**
 * @brief Initializes the environment array cache in the shell structure
 * 
 * @param shell Pointer to shell structure
 */
void	init_env_cache(t_shell *shell)
{
	shell->env_array = NULL;
	shell->env_modified = true;
}

/**
 * @brief Cleans up the environment array cache
 * 
 * @param shell Pointer to shell structure
 */
void	cleanup_env_cache(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->env_array)
	{
		ft_free_array(shell->env_array);
		shell->env_array = NULL;
	}
	shell->env_modified = true;
}

/**
 * @brief Marks the environment as modified, triggering a rebuild of the cache
 * 
 * @param shell Pointer to shell structure
 */
void	mark_env_modified(t_shell *shell)
{
	if (shell->env_array)
	{
		ft_free_array(shell->env_array);
		shell->env_array = NULL;
	}
	shell->env_modified = true;
}

/**
 * @brief Gets the environment array, rebuilding it if necessary
 * 
 * @param shell Pointer to shell structure
 * @return char** The environment array
 */
char	**get_env_array(t_shell *shell)
{
	shell->env_array = create_env_array(shell->env);
	if (!shell->env_array)
		return (NULL);
	shell->env_modified = false;
	return (shell->env_array);
}
