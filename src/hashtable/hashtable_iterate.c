#include "hashtable.h"

void hashmap_iterate(t_hashmap *table, void (*f)(const char *, const char *))
{
    size_t i;
    t_hash_item *item;

    if (!table || !f)
        return;
    
    for (i = 0; i < table->size; i++)
    {
        item = table->items[i];
        while (item)
        {
            f(item->key, item->value);
            item = item->next;
        }
    }
}