#include "hashtable.h"

void	hashmap_free_item(t_hash_item *item)
{
	free (item->key);
	free (item->value);
	free (item);
}