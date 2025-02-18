/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_check.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:50:38 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 14:50:57 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef COMMAND_CHECK_H
# define COMMAND_CHECK_H

int		handle_direct_path(t_shell *shell, const char *cmd);
int		is_special_path(const char *cmd);
int		is_direct_path(const char *cmd);
char	*get_path_env(t_shell *shell);
char	*find_command(t_shell *shell, const char *cmd);
char	**split_path_env(t_shell *shell, const char *path);
char	*search_in_paths(t_shell *shell, char **paths, const char *cmd);
char	*try_path(const char *path, const char *cmd);
char	*get_command_path(t_shell *shell, const char *cmd, t_hashmap *env);
int		handle_path_resolution(t_shell *shell, t_ast_node *node, \
						char **cmd_path);
#endif
