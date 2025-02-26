/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:51:11 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:51:24 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPANDER_H
# define EXPANDER_H

/* Gllobber Submodule*/
size_t	ft_mutli_arr_strs_count(char ***strs_arrays);
char	**ft_flattern_str_arrays(char ***str_arrays);
size_t	ft_pattern_match_count(char *pattern);
char	**ft_globber(char **expanded);
/* End of Globber Submodule*/
/* Quote Handler Submodule*/
char	*normal_str_handler(char *str, size_t *i);
char	*single_quotes_handler(char *str, size_t *i);
char	*double_quote_str(t_shell *shell, char *str, size_t *i);
char	*double_quotes_handler(t_shell *shell, char *str, size_t *i);
/* End of Quote Handler Submodule*/
/* Quote Stripper Submodule*/
void	fill_unquoted_str(char *str, size_t *i, char *ret, size_t *j);
size_t	ft_unquoted_strlen(char *str);
char	*ft_strip_quotes(char *str);
/* End of Quote Stripper Submodule*/
/* Token Expander Submodule*/
char	*clean_empty_strs(char *str);
char	**expand_args(t_shell *shell, char *str);
/* End of Token Expander Submodule*/
/* Variable Expander Submodule */
bool	is_valid_var_char(char c);
char	*dollar_handler(t_shell *shell, char *str, size_t *i);
/* End of Variable Expander Submodule */
/* Wildcard Matcher Submodule */
bool	is_pattern_match(char **pattern, char **last_wildcard, \
					char **last_match, char **str);
void	pattern_quotes_handler(char **pattern, char *quotes);
bool	wildcard_handler(char **pattern, char **last_wildcard, \
					char **last_match, char *str);
bool	ft_is_wildcard(char *pattern, char *str);
/* End of Wildcard Matcher Submodule */
/* Word Splitter Submodule */
char	**expand_and_split(char *str);
void	fill_words(const char *str, char **array, size_t *i, size_t j);
char	**ft_filler(const char *str, char **array);
#endif
