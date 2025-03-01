/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 21:39:35 by szhong            #+#    #+#             */
/*   Updated: 2025/02/28 22:22:20 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "builtins/builtins.h"

static int	export_error(char *identifier)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(identifier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static void	export_list(t_shell *shell)
{
	t_hash_item	*next;
	t_hash_item	*current;
	size_t		i;
	size_t		j;

	i = 0;
	while (i < shell->env->size)
	{
		current = shell->env->items[i];
		while (current != NULL)
		{
			if (current->value != NULL && (ft_strcmp(current->key, "_") != 0))
			{
				ft_printf("=\"");
				j = 0;
				while ((current->value)[j])
				{
					if  ((current->value)[j] == $ || (current->value[j] == '"'))
						ft_printf("\\%c", (current->value)[j++]);
					else
						ft_printf("%c", (current->value)[j++]);
				}
				ft_printf("\"\n");
			}
			current = current->next;
		}
		i++;
	}
}

bool	check_valid_key(char *key)
{
	int	i;

	i = 0;
	if (!ft_isalpha(key[i]) && key[i] != '_')
		return (false);
	while (key[i] && key[i] != '=')
	{
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	builtin_export(t_shell *shell, char **argv)
{
	int	i;
	int	exit_status;
	char	*key;
	bool	has_env_key;

	exit_status = 0;
	i = 1;
	if (!argv[1])
		return (export_list(shell));
	while (argv[i])
	{
		if (!check_valid_key(argv[i]))
			exit_status = export_error(argv[i]);
		else
		{
			key = dup_key(shell, argv[i]);
			if (key)
				hashmap_replace_value(shell, shell->env, key, dup_value(shell, argv[i]));
			else
				hashmap_insert(shell, (t_hashmap_insert_params){
					.table = shell->env, .key = key, .value = dup_value(shell, argv[i])});
			i++;
		}
	}
	
}

