/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashtable_murmurhash2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: szhong <szhong@student.42london.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:15 by szhong            #+#    #+#             */
/*   Updated: 2024/12/19 16:45:14 by szhong           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"
#include "hashtable.h"
#include "libft.h"

size_t hash_function(const char *key, size_t table_size)
{
    const unsigned int m;
    const unsigned int seed;
    const int r;
    unsigned int h;
    const unsigned char *data = (const unsigned char *)key;

    (m = 0x5bd1e995, seed = 0, r = 24);
    h = seed ^ ft_strlen(key);
    while (*key)
    {
        unsigned int k = *data++;
        k *= m;
        k ^= k >> r;
        k *= m;
        h *= m;
        h ^= k;
        key++;
    }
    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;
    return (h % table_size);
}
