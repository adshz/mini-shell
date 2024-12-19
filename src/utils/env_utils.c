#include <stdlib.h>
#include "env_utils.h"
#include "hashtable.h"
#include "libft.h"

char	*get_env_value(t_env *env, const char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

static t_env	*create_env_node(const char *key, const char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	new_node->value = ft_strdup(value);
	if (!new_node->value)
	{
		free(new_node->key);
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

t_env	*hashmap_to_env(t_hashmap *map)
{
	t_env		*env_list;
	t_env		*new_node;
	t_hash_item	*item;
	size_t		i;

	env_list = NULL;
	i = 0;
	while (i < map->size)
	{
		item = map->items[i];
		while (item)
		{
			if (item->key && item->value)
			{
				new_node = create_env_node(item->key, item->value);
				if (!new_node)
					return (NULL);
				new_node->next = env_list;
				env_list = new_node;
			}
			item = item->next;
		}
		i++;
	}
	return (env_list);
}

void	free_env_list(t_env *env)
{
	t_env	*current;
	t_env	*next;

	current = env;
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
} 