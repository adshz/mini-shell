/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 15:50:40 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:45:46 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef HASHTABLE_H
# define HASHTABLE_H
# define HASH_SIZE 1024

# include "libft.h"
# include <stdlib.h>

typedef struct s_hash_item {
    char *key;
    char *value;
    struct s_hash_item *next;
} t_hash_item;

typedef struct s_hashmap
{
    t_hash_item **items;
    size_t size;
    size_t count;
} t_hashmap;

size_t		hash_function(const char *key, size_t table_size);
t_hashmap    *hashmap_create_table(size_t size);
t_hashmap    *env_to_hash(char **envp);
void        hashmap_destroy(t_hashmap *table);
int         hashmap_insert(t_hashmap *table, char *key, char *value, int free_old);
char        *hashmap_search(t_hashmap *table, char *key);
void        hashmap_delete(t_hashmap *table, char *key);
t_hashmap    *hashmap_create(void);
void        hashmap_set(t_hashmap *table, const char *key, const char *value);
char        *hashmap_get(t_hashmap *table, const char *key);
void        hashmap_remove(t_hashmap *table, const char *key);
void        hashmap_iterate(t_hashmap *table, void (*f)(const char *, const char *));
size_t      hashmap_size(t_hashmap *table);
char        *ft_strjoin3(const char *s1, const char *s2, const char *s3);
void        ft_free_array(char **array);
void hashmap_handle_collision(t_hashmap *table, size_t index, 
                            t_hash_item *new_item);

#endif
