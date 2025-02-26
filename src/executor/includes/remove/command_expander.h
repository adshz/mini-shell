/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_expander.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:16:00 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:16:03 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_EXPANDER_H
# define COMMAND_EXPANDER_H

/* Command name expansion */
int		expand_command_name_with_var(t_shell *shell,
			t_ast_node *node, char *dollar_pos);
int		handle_expanded_command(char *expanded, char *dollar_pos,
			t_ast_node *node);

/* Argument expansion */
int		process_remaining_args(t_shell *shell, t_ast_node *node);
int		process_single_arg_expansion(t_shell *shell, char **arg);
int		replace_argument(char **arg, char *expanded);
int		handle_expansion(t_shell *shell, char *dollar_pos,
			char **expanded, bool *was_in_double_quotes);

/* Path expansion */
int		handle_tilde_directory(const char *expanded);
int		handle_tilde_path(t_shell *shell, t_ast_node *node);
int		handle_dot_paths(t_shell *shell, const char *arg);
int		handle_special_paths(t_shell *shell, t_ast_node *node);
char	*expand_tilde(t_shell *shell, const char *arg);

/* Command name expansion utils */
size_t	cmd_count_command_parts(char **split_parts);
size_t	cmd_count_remaining_args(char **args);
char	**cmd_allocate_expanded_args(size_t total_size);
void	cmd_copy_command_parts(char **dest, char **src, size_t count);
int		cmd_copy_remaining_args(char **dest, char **src,
			size_t dest_start, size_t count);
int		build_new_args_array(t_ast_node *node, char **split_parts);

/* String manipulation utils */
char	*get_prefix(const char *arg, char *dollar_pos);
char	*get_suffix(char *dollar_pos);
char	*combine_parts(char *prefix, char *expanded, char *suffix);
void	init_quote_state(t_shell *shell, const char *arg,
			bool *was_in_double_quotes);
int		handle_arg_split(t_ast_node *node, int *i, char *space_pos);

/* Command name prefix handling */
int		cmd_create_command_prefix(const char *arg, size_t prefix_len,
			char **prefix);
int		cmd_handle_prefix_extraction(const char *arg, char *dollar_pos,
			char **prefix);
char	*cmd_extract_variable_name(char *dollar_pos, char **var_end);
char	*cmd_handle_variable_expansion(t_shell *shell, char *dollar_pos,
			char **var_end);
char	*cmd_ex_handle_exit_status(t_shell *shell);

int		expand_command_args(t_shell *shell, t_ast_node *node);
#endif 
