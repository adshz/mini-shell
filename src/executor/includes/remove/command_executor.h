/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:30 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:33 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_EXECUTOR_H
# define COMMAND_EXECUTOR_H

/* Command execution */
int		execute_ast(t_shell *shell, t_ast_node *node);
int		execute_command(t_shell *shell, t_ast_node *node);
int		execute_external_command(t_shell *shell, t_ast_node *node);
void	execute_ast_node(t_shell *shell, t_ast_node *node);

/* Command path handling */
int		is_direct_path(const char *cmd);
char	*get_path_env(t_shell *shell);
int		is_special_path(const char *cmd);
int		handle_direct_path(t_shell *shell, const char *cmd);
char	*find_command(t_shell *shell, const char *cmd);
char	**split_path_env(t_shell *shell, const char *path);
char	*search_in_paths(t_shell *shell, char **paths, const char *cmd);
char	*try_path(const char *path, const char *cmd);
char	*get_command_path(t_shell *shell, const char *cmd, t_hashmap *env);

/* Command process handling */
int		execute_command_with_path(t_shell *shell, t_ast_node *node,
			char *cmd_path);
int		execute_command_child(t_ast_node *node,
			char *cmd_path, char **env_array);
int		handle_parent_process(t_shell *shell, pid_t pid,
			char *cmd_path);

/* Command error handling */
int		handle_command_not_found(t_shell *shell, const char *cmd);
void	handle_child_error(const char *cmd);
int		check_command(t_shell *shell, t_ast_node *cmd);
int		validate_and_expand_command(t_shell *shell, t_ast_node *node);

int		execute_external_command(t_shell *shell, t_ast_node *node);
int		handle_external_parent_process(pid_t pid);
int		command_executor_execute_child_process(t_shell *shell, \
												t_ast_node *node, \
												char *cmd_path, \
												char **env_array);
#endif 
