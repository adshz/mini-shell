/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/16 21:58:54 by szhong            #+#    #+#             */
/*   Updated: 2025/02/16 21:59:35 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_H
# define ENV_H

# include "hashtable/hashtable.h"
# include "utils/utils.h"
# include "executor/executor.h"
# include <sys/wait.h>

void	env_print_env_var(const char *key, const char *value);
int		handle_env_parent_process(pid_t pid, char *cmd_path);
int		handle_env_child_process(char *cmd_path, \
							char **args, char **env_array);
int		handle_env_command_not_found(const char *cmd);
int		builtin_env(t_shell *shell, t_ast_node *node);

#endif
