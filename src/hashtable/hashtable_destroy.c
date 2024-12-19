static void free_item_chain(t_hash_item *item)
{
    t_hash_item *next;

    while (item)
    {
        next = item->next;
        free(item->key);
        free(item->value);
        free(item);
        item = next;
    }
}

void hashmap_destroy(t_hashmap *table)
{
    size_t i;

    if (!table)
        return;
    for (i = 0; i < table->size; i++)
        free_item_chain(table->items[i]);
    free(table->items);
    free(table);
} 