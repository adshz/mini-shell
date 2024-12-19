#include "hashtable.h"

void hashmap_handle_collision(t_hashmap *table, size_t index, 
                            t_hash_item *new_item)
{
    t_hash_item *current;

    current = table->items[index];
    while (current->next != NULL)
        current = current->next;
    current->next = new_item;
    new_item->next = NULL;
    table->count++;
} 