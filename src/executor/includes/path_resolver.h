/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_resolver.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 21:34:47 by szhong            #+#    #+#             */
/*   Updated: 2025/02/26 21:35:03 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PATH_RESOLVER_H
# define PATH_RESOLVER_H
# include "../executor.h"

t_path	get_path(t_shell *shell, char *cmd);
t_path	get_env_path(t_shell *shell, char *path, char *cmd);
t_err	check_executable(char *file, bool is_cmd);
#endif
