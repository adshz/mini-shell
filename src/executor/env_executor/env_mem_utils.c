#include "executor/executor.h"

void cleanup_partial_array(char **array, size_t count)
{
	size_t i;

	if (!array)
		return;
	i = 0;
	while (i < count)
	{
		if (array[i])
		{
			free(array[i]);
			array[i] = NULL;
		}
		i++;
	}
}