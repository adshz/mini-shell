/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 16:03:06 by szhong            #+#    #+#             */
/*   Updated: 2025/02/17 16:03:07 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef EXPORT_H
# define EXPORT_H
# include "libft.h"
# include "types.h"
# include "hashtable/hashtable.h"
# include "builtins/builtin_utils/builtin_utils.h"
# include "expander/expander.h"
# include <stdlib.h>

void	export_handle_expanded_value(t_shell *shell, char *key, char *value);
char	*export_handle_quoted_value(char *value);
void	export_with_value(t_shell *shell, char *key, char *value);
void	handle_export_no_equals(t_shell *shell, char *arg);
void	process_export_argument(t_shell *shell, char *arg);
bool	export_check_expanded_arg(t_shell *shell, char *arg);
int		export_handle_invalid_identifier(char *arg);
void	export_print_env_var(const char *key, const char *value);

#endif
