/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_executor.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 02:15:50 by szhong            #+#    #+#             */
/*   Updated: 2025/02/18 02:15:53 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_EXECUTOR_H
# define ENV_EXECUTOR_H

# include "hashtable/hashtable.h"

t_hash_item	*get_next_valid_item(t_hash_item *current);
/* Environment string handling */
int			validate_env_string_input(const char *key, const char *value, \
							size_t *key_len, size_t *value_len);
char		*build_env_string(const char *key, const char *value, \
						size_t key_len, size_t value_len);
char		*create_env_string(const char *key, const char *value);

/* Environment array handling */
char		**create_env_array(t_hashmap *env);
int			fill_env_array(t_hashmap *env, char **env_array);
void		cleanup_partial_array(char **array, size_t count);

/* Environment array caching */
void		init_env_cache(t_shell *shell);
void		cleanup_env_cache(t_shell *shell);
char		**get_env_array(t_shell *shell);
void		mark_env_modified(t_shell *shell);

/* Environment variable handling */
int			handle_variable_assignment(t_shell *shell, const char *assignment);
int			handle_variable_expansion(t_shell *shell, t_ast_node *node);
int			process_variable_expansion(t_shell *shell, \
								char *dollar_pos, char **expanded);
int			convert_exit_status_to_str(t_shell *shell, char **expanded);

/* Hashmap operations */
int			iterate_hash_buckets(t_hashmap *env, char **env_array);
int			process_bucket_items(char **env_array, size_t *index, \
						t_hash_item *item);
int			add_env_item(char **env_array, size_t *index, \
				t_hash_item *item);
int			process_all_hash_buckets(t_hashmap *env, char **env_array, \
							size_t *array_index);
int			validate_hashmap(t_hashmap *env, char **env_array);
int			is_pointer_valid(void *ptr);
int			is_hash_item_valid(t_hash_item *item);
int			process_single_item(char **env_array, \
						size_t *index, t_hash_item *current);

#endif 
