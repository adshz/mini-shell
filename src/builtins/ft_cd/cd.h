/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: evmouka <evmouka@student.42london.com>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 19:14:56 by evmouka           #+#    #+#             */
/*   Updated: 2025/02/12 19:15:29 by evmouka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef CD_H
# define CD_H

# include <unistd.h>
# include <limits.h>
# include "../../inc/types.h"
# include "../../inc/shell.h"
# include "hashtable/hashtable.h"
# include "libft.h"

/* Error messages */
# define ERR_GETCWD "cd: error retrieving current directory: "
# define ERR_FILE "getcwd: cannot access parent directories"
# define ERR_OLDPWD_NOT_SET "cd: OLDPWD not set"
# define ERR_HOME_NOT_SET "cd: HOME not set"
# define ERR_NO_FILE "No such file or directory"

/* CD utilities that were in builtins.h */
char	*get_oldpwd_path(t_shell *shell);
char	*get_home_path(t_shell *shell);
int		handle_cd_path(t_shell *shell, const char *path);

/* Internal CD functions */
void	update_pwd_vars(t_shell *shell, char *old_pwd);
int		handle_cd_error(const char *path, const char *arg);
void	cleanup_path(char *path, const char *arg, t_shell *shell);
char	*expand_tilde(t_shell *shell, const char *arg);
char	*handle_path(t_shell *shell, char *arg);

#endif
