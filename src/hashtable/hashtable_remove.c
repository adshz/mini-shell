#include "hashtable.h"

static void free_item(t_hash_item *item)
{
    free(item->key);
    free(item->value);
    free(item);
}

void hashmap_remove(t_hashmap *table, const char *key)
{
    size_t index;
    t_hash_item *current;
    t_hash_item *prev;

    if (!table || !key)
        return;
    index = hash_function(key, table->size);
    current = table->items[index];
    prev = NULL;
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
        {
            if (prev)
                prev->next = current->next;
            else
                table->items[index] = current->next;
            free_item(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}