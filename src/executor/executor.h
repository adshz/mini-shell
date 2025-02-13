/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:34:37 by szhong            #+#    #+#             */
/*   Updated: 2025/02/13 17:18:46 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "libft.h"
# include "shell.h"
# include "core/core.h"
# include "errors.h"
# include <sys/wait.h>
# include <errno.h>
# include "types.h"
# include "expander/expander.h"
# include <signal.h>
# include "hashtable/hashtable.h"
# include <stdlib.h>   // for malloc
# include <stdint.h>   // for SIZE_MAX

extern volatile sig_atomic_t	g_signal_status;
void	debug_heredoc_state(t_ast_node *node,
			const char *line, const char *stage);

typedef struct s_heredoc_content
{
	char	**content;
	size_t	*content_size;
	size_t	*content_capacity;
	t_shell	*shell;
}	t_heredoc_content;

typedef struct s_heredoc_data
{
	char		*content_path;
	int			content_fd;
	const char	*delimiter;
}	t_heredoc_data;

/* Heredoc data management */
int		init_heredoc_data(t_heredoc_data *data, const char *delimiter);
void	cleanup_heredoc_data(t_heredoc_data *data);
void	cleanup_heredoc_resources(t_heredoc_content *hdc);

/* Command execution */
int		execute_ast(t_shell *shell, t_ast_node *node);
int		execute_command(t_shell *shell, t_ast_node *node);
int		execute_external_command(t_shell *shell, t_ast_node *node);

/* Pipeline handling */
void	execute_ast_node(t_shell *shell, t_ast_node *node);
int		execute_pipe(t_shell *shell, t_ast_node *node);
void	close_pipe(int pipefd[2]);
int		setup_pipe(int pipe_fd[2]);

/* Redirection handling */
void	setup_redirections(t_shell *shell, t_ast_node *node);
void	restore_redirections(t_shell *shell);
int		execute_redirection(t_shell *shell, t_ast_node *node);
int		handle_heredoc(t_ast_node *node, t_shell *shell);

/* Environment handling */
char	**create_env_array(t_hashmap *env);
int		fill_env_array(t_hashmap *env, char **env_array);

/* Command history functions */
void	add_command_history(t_shell *shell, void *ast);

int		iterate_hash_buckets(t_hashmap *env, char **env_array);
int		process_bucket_items(char **env_array, size_t *index,
			t_hash_item *item);
int		add_env_item(char **env_array, size_t *index, t_hash_item *item);
int		add_env_item(char **env_array, size_t *index, t_hash_item *item);
int		process_all_hash_buckets(t_hashmap *env, char **env_array,
			size_t *array_index);
void	cleanup_partial_array(char **array, size_t count);
int		validate_hashmap(t_hashmap *env, char **env_array);
int		handle_variable_assignment(t_shell *shell, const char *assignment);
int		is_pointer_valid(void *ptr);
int		handle_expanded_command(char *expanded, char *dollar_pos,
			t_ast_node *node);

int		handle_tilde_directory(const char *expanded);
int		handle_tilde_path(t_shell *shell, t_ast_node *node);
int		handle_dot_paths(t_shell *shell, const char *arg);
int		handle_special_paths(t_shell *shell, t_ast_node *node);
char	*expand_tilde(t_shell *shell, const char *arg);

int		handle_variable_expansion(t_shell *shell, t_ast_node *node);
int		expand_command_args(t_shell *shell, t_ast_node *node);

int		handle_command_not_found(t_shell *shell, const char *cmd);
int		validate_and_expand_command(t_shell *shell, t_ast_node *node);

int		execute_command_with_path(t_shell *shell, t_ast_node *node, \
			char *cmd_path);
int		execute_command_child(t_ast_node *node, \
			char *cmd_path, char **env_array);
int		handle_parent_process(t_shell *shell, pid_t pid, \
			char *cmd_path, char **env_array);

int		is_direct_path(const char *cmd);
char	*get_path_env(t_shell *shell);
int		is_special_path(const char *cmd);
int		handle_direct_path(t_shell *shell, const char *cmd);

char	*find_command(t_shell *shell, const char *cmd);
char	**split_path_env(t_shell *shell, const char *path);
char	*search_in_paths(t_shell *shell, char **paths, const char *cmd);

char	*try_path(const char *path, const char *cmd);
char	*get_command_path(t_shell *shell, const char *cmd, t_hashmap *env);

int		handle_node_by_type(t_shell *shell, t_ast_node *node);

int		handle_cd_redirection(t_shell *shell, t_ast_node *node);
int		handle_other_builtin_redirection(t_shell *shell, t_ast_node *node);
int		handle_builtin_redirection(t_shell *shell, t_ast_node *node);
int		handle_redirection_node(t_shell *shell, t_ast_node *node);

void	execute_left_child(t_shell *shell, t_ast_node *node, int pipe_fd[2]);
void	execute_right_child(t_shell *shell, t_ast_node *node, int pipe_fd[2]);

int		execute_left_process(t_shell *shell, t_ast_node *node, int pipe_fd[2]);
int		execute_right_process(t_shell *shell, t_ast_node *node, \
			int pipe_fd[2], int *right_status);
int		handle_fork_error(int pipe_fd[2]);

void	handle_child_error(const char *cmd);
int		check_command(t_shell *shell, t_ast_node *cmd);

int		handle_path_resolution(t_shell *shell,
			t_ast_node *node, char **cmd_path);
char	*create_env_string(const char *key, const char *value);

int		append_line_to_content(char **content, size_t *content_size, \
			char *line, size_t *content_capacity);

char	*expand_redirection_filename_variable(t_shell *shell,
			const char *filename);
int		read_heredoc_content(t_ast_node *node, t_heredoc_content *hdc);
int		init_heredoc_content(char **content);
int		resize_content_buffer(char **content, size_t *content_capacity, \
			size_t needed_size, char *line);
int		setup_pipe_and_write(t_heredoc_data *data);

/* Redirection filename handling */
char	*handle_quoted_filename(t_shell *shell, const char *filename);
char	*handle_quoted_variable(t_shell *shell, char *unquoted);
char	*remove_quotes(const char *filename);
char	*get_expanded_filename(t_shell *shell, const char *filename);
int		is_ambiguous_redirect(t_shell *shell, const char *value);
int		handle_ambiguous_filename(const char *filename);
int		handle_output_redirection(t_ast_node *node, t_shell *shell);
int		handle_append_redirection(t_ast_node *node, t_shell *shell);
int		handle_input_redirection(t_ast_node *node, t_shell *shell);

// command name expansion flow
int		expand_command_name_with_var(t_shell *shell,
			t_ast_node *node, char *dollar_pos);
// argument expansion flow
int		process_remaining_args(t_shell *shell, t_ast_node *node);
int		handle_expansion(t_shell *shell, char *dollar_pos,
			char **expanded, bool *was_in_double_quotes);
int		process_single_arg_expansion(t_shell *shell, char **arg);
int		replace_argument(char **arg, char *expanded);
char	*get_prefix(const char *arg, char *dollar_pos);
char	*get_suffix(char *dollar_pos);
char	*combine_parts(char *prefix, char *expanded, char *suffix);
void	init_quote_state(t_shell *shell, const char *arg,
			bool *was_in_double_quotes);
int		handle_arg_split(t_ast_node *node, int *i, char *space_pos);

int		get_signal_from_status(int status);
int		get_exit_status(int status);
int		was_signaled(int status);

// external command signals
void	setup_child_process(void);
void	setup_parent_process(void);
int		handle_signal_termination(t_shell *shell, int status);

// command name expander utils
size_t	cmd_count_command_parts(char **split_parts);
size_t	cmd_count_remaining_args(char **args);
char	**cmd_allocate_expanded_args(size_t total_size);
void	cmd_copy_command_parts(char **dest, char **src, size_t count);
int		cmd_copy_remaining_args(char **dest, char **src,
			size_t dest_start, size_t count);
int		build_new_args_array(t_ast_node *node, char **split_parts);

// command name expander 
// phase 1 prefix handling
int		cmd_create_command_prefix(const char *arg, size_t prefix_len,
			char **prefix);
int		cmd_handle_prefix_extraction(const char *arg, char *dollar_pos,
			char **prefix);
char	*cmd_extract_variable_name(char *dollar_pos, char **var_end);

// phase 2 command vairable expansiion
char	*cmd_handle_variable_expansion(t_shell *shell, char *dollar_pos,
			char **var_end);
char	*cmd_ex_handle_exit_status(t_shell *shell);

#endif