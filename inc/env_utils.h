/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:51:23 by szhong            #+#    #+#             */
/*   Updated: 2024/12/18 15:51:23 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef ENV_UTILS_H
# define ENV_UTILS_H

# include "hashtable.h"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

char	*get_env_value(t_env *env, const char *key);
t_env	*hashmap_to_env(t_hashmap *map);
void	free_env_list(t_env *env);

#endif 