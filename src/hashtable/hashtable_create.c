#include "hashtable.h"

t_hashmap *hashmap_create(void)
{
    t_hashmap *table;
    
    table = malloc(sizeof(t_hashmap));
    if (!table)
        return (NULL);
    table->size = HASH_SIZE;
    table->count = 0;
    table->items = calloc(table->size, sizeof(t_hash_item *));
    if (!table->items)
    {
        free(table);
        return (NULL);
    }
    return (table);
}