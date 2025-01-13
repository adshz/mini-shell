/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "hashtable.h"
# include "shell.h"

// Environment initialization functions
t_hashmap    *init_env(char **envp);
t_hashmap    *init_hashmap(void);

// Environment variable management
void        init_env_vars(t_shell *shell, char *argv[]);
void        init_shlvl(t_shell *shell);
void        init_pwd(t_shell *shell);

#endif 