#include "hashtable.h"

size_t hashmap_size(t_hashmap *table)
{
    size_t size = 0;
    size_t i;

    if (!table)
        return 0;
    for (i = 0; i < table->size; i++)
    {
        t_hash_item *item = table->items[i];
        while (item)
        {
            size++;
            item = item->next;
        }
    }
    return size;
}