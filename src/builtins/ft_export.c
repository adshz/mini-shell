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
				ft_printf("%s=\"", current->key);
				j = 0;
				while ((current->value)[j])
				{
					if  ((current->value)[j] == '$' || \
						(current->value[j] == '"'))
						ft_printf("\\%c", (current->value)[j++]);
					else
						ft_printf("%c", (current->value)[j++]);
				}
				ft_printf("\"\n");
			}
			else if (current->value == NULL && (ft_strcmp(current->key, "_") != 0))
				ft_printf("%s\n", current->key);
			current = current->next;
		}
		i++;
	}
}

static char *get_key_from_arg(char *arg)
{
	char	*equals_pos;
	char	*key;

	equals_pos = ft_strchr(arg, '=');
	if (!equals_pos)
		return (ft_strdup(arg));
	key = ft_substr(arg, 0, equals_pos - arg);
	return (key);
}

int	check_valid_key(char *arg)
{
	char	*key;
	int		i;
	int		valid;

	key = get_key_from_arg(arg);
	if (!key)
		return (0);

	valid = 1;
	i = 0;
	if (!ft_isalpha(key[0]) && key[0] != '_')
		valid = 0;
	else
	{
		while (key[i] && valid)
		{
			if (!ft_isalnum(key[i]) && key[i] != '_')
				valid = 0;
			i++;
		}
	}
	free(key);
	return (valid);
}

static char *join_value_args(char **argv, int start_idx)
{
	char	*value;
	char	*tmp;
	int		i;

	if (!argv[start_idx])
		return (ft_strdup(""));
	
	value = ft_strdup(argv[start_idx]);
	if (!value)
		return (NULL);

	i = start_idx + 1;
	while (argv[i])
	{
		tmp = value;
		value = ft_strjoin(value, argv[i]);
		free(tmp);
		if (!value)
			return (NULL);
		i++;
	}
	return (value);
}

int	builtin_export(t_shell *shell, char **argv)
{
	char	*key;
	char	*value;
	int		i;
	int		exit_status;

	exit_status = 0;
	i = 1;
	if (!argv[1])
		return ((export_list(shell), 0));

	while (argv[i])
	{
		ft_putstr_fd("DEBUG [export]: Processing argument: ", 2);
		ft_putstr_fd(argv[i], 2);
		ft_putstr_fd("\n", 2);

		if (ft_strchr(argv[i], '='))
		{
			key = get_key_from_arg(argv[i]);
			if (!check_valid_key(argv[i]))
			{
				exit_status = export_error(argv[i]);
				free(key);
			}
			else
			{
				value = join_value_args(argv, i + 1);
				ft_putstr_fd("DEBUG [export]: Key: ", 2);
				ft_putstr_fd(key, 2);
				ft_putstr_fd(", Value: ", 2);
				ft_putstr_fd(value ? value : "(null)", 2);
				ft_putstr_fd("\n", 2);

				if (hashmap_search(shell->env, key) != NULL)
					hashmap_replace_value(shell, shell->env, key, value);
				else
					hashmap_insert(shell, (t_hashmap_insert_params){
						.table = shell->env,
						.key = key,
						.value = value
					});
			}
			break;  // Stop processing after handling key=value
		}
		else if (!check_valid_key(argv[i]))
		{
			exit_status = export_error(argv[i]);
		}
		else
		{
			key = ft_strdup(argv[i]);
			if (hashmap_search(shell->env, key) == NULL)
				hashmap_insert(shell, (t_hashmap_insert_params){
					.table = shell->env,
					.key = key,
					.value = NULL
				});
			else
				free(key);
		}
		i++;
	}
	return (exit_status);
}
